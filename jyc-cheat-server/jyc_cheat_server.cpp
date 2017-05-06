#include "jyc_cheat_server.hpp"


using namespace std;
using namespace jyc_cheat;
using json = nlohmann::json;


jyc_cheat_server::jyc_cheat_server(){
  this->server_sock = 0;
  FD_ZERO(&this->rfds);
  this->MakeTestData();
}


jyc_cheat_server::~jyc_cheat_server(){
}


string jyc_cheat_server::CurrentServerState(){
  return NULL;
}


int jyc_cheat_server::MakeTestData(){
  cheat_room* room1 = new cheat_room("test_room1");
  cheat_room* room2 = new cheat_room("test_room2");
  cheat_room* room3 = new cheat_room("test_room3");
  cheat_room* room4 = new cheat_room("test_room4");

  this->map_rooms["test_room1"] = room1;
  this->map_rooms["test_room2"] = room2;
  this->map_rooms["test_room3"] = room3;
  this->map_rooms["test_room4"] = room4;

  return 0;
}


int jyc_cheat_server::StartServer(){
  printf("Start Server\n");
  this->StartListen();
  this->StartAccept();
  return 0;
}


int jyc_cheat_server::StartListen(){
  printf("Start Listening....\n");
  int state = 0;
  int option = 1;

  this->server_sock = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(this->server_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->server_addr.sin_port = htons(atoi("9999"));

  state = ::bind(this->server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  printf("bind state %d\n", state);

  if (state == -1){
    printf("bind error\n");
    perror("bind error : ");
    exit(0);
  }

  state = listen(this->server_sock, 5);
  printf("listen state %d\n", state);
  if (state == -1){
    printf("listen error\n");
    perror("listen error : ");
    exit(0);
  }
  
  return 0;
}


int jyc_cheat_server::StartAccept(){

  int client_sock = 0;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);
  int flags;

  this->StartConsumeMessages();
  this->StartReceivingMessages();

  cout << "Start Accepting..." << endl;
  while(true){
    client_sock = accept(this->server_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    flags = fcntl(this->server_sock, F_GETFL, 0);
    fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);
    cout << "connected client : " << client_sock << endl;
    cheat_user* user = new cheat_user(client_sock);

    this->list_user.push_back(user);
    cout << this->list_user.size() << endl;
  }

  pthread_join(this->thread_check_disconnection, NULL);
  pthread_join(this->thread_message_consummer, NULL);
  
  return 0;
}


int jyc_cheat_server::RemoveUser(cheat_user* user){
  list<cheat_user*>::iterator iter;
  for(iter = this->list_user.begin(); iter != this->list_user.end(); ++iter){
    if(user->GetMainSock() == (*iter)->GetMainSock()){
      this->list_user.erase(iter);
    }
  }

  return 0;
}


int jyc_cheat_server::StartConsumeMessages(){
  pthread_create(&this->thread_message_consummer, NULL, jyc_cheat_server::ConsumeMessagesLogic, (void*)this);
  return 0;
}


void *jyc_cheat_server::ConsumeMessagesLogic(void* t){

  jyc_cheat_server* server = (jyc_cheat_server*)*&t;

  while(true){
    usleep(100);
    if(server->queue_message.size() > 0){
      cheat_message message = server->queue_message.front();
      server->queue_message.pop();
      printf("I got a message in the loop : %s\n", message.GetMessage().c_str());
      server->MessageHandler(server, &message);
    }
  }
}

int jyc_cheat_server::MessageHandler(jyc_cheat_server* server, cheat_message* message){
  json received_json;
  string req_tags;

  try{
    received_json  = json::parse(message->GetMessage());
  }
  catch (...) {
    cout << "error occured during parsing message" << endl;
    return -1;
  }

  req_tags = received_json["req_tag"].get<std::string>();
  cout << "req_tags : " << req_tags << endl;
  int result_of_send = 0;

  if(req_tags.compare("get_room_list") == 0){
    result_of_send = this->HandleRoomList(server, message);
  }
  else if(req_tags.compare("is_existing_the_room") == 0){
    result_of_send = this->HandleIsExistingTheRoom(server, message);
  }
  else if(req_tags.compare("create_the_room_with_nickname") == 0){
    result_of_send = this->HandleCreateTheRoomWithNickName(server, message);
  }
  else if(req_tags.compare("enter_the_room_with_nickname") == 0){
    result_of_send = this->HandleEnterTheRoomWithNickName(server, message);
  }
  else if(req_tags.compare("send_cheat_message") == 0){
    result_of_send = this->HandleSendCheatMessage(server, message);
  }
  else{
    return 0;
  }

  // write failed... In this case, the socket was closed. So should remove connected user.
  if (result_of_send == -1){
    this->RemoveUser(message->GetUser());
  }

  return 0;
}


int jyc_cheat_server::HandleRoomList(jyc_cheat_server* server, cheat_message* message){
  string message_to_send;
  // make room list data
  json json_ret;
  json json_rooms;
  map<string, cheat_room*>::iterator iter;
  for (iter = server->map_rooms.begin(); iter != server->map_rooms.end(); iter++){
    cout << iter->first << endl;
    json_rooms.push_back(iter->first);
  }
  json_ret["res_tag"] = "get_room_list";
  json_ret["rooms"] = json_rooms;

  return message->GetUser()->SendMessageWithMain(json_ret.dump());
}


int jyc_cheat_server::HandleIsExistingTheRoom(jyc_cheat_server* server, cheat_message* message){
  json json_recv = json::parse(message->GetMessage());
  string room_name = json_recv["room_name"].get<string>();
  bool is_exist = false;

  map<string, cheat_room*>::iterator iter;
  for (iter = server->map_rooms.begin(); iter != server->map_rooms.end(); iter++){
    cout << iter->first << endl;
    if(iter->first.compare(room_name) == 0){
      is_exist = true;
      break;
    }
  }

  json json_ret;
  json_ret["res_tag"] = "make_the_room";
  json json_exist;

  if (is_exist){
    //Make the room and result message
    json_ret["is_exist"] = true;
  }
  else{
    //Make the room and result message
    json_ret["is_exist"]= false;
  }

  return message->GetUser()->SendMessageWithMain(json_ret.dump());
}

int jyc_cheat_server::HandleCreateTheRoomWithNickName(jyc_cheat_server *server, cheat_message* message){
  json json_recv = json::parse(message->GetMessage());
  string room_name = json_recv["room_name"].get<string>();
  string nick_name = json_recv["nickname"].get<string>();

  json json_ret;
  json_ret["res_tag"] = "create_the_room_with_nickname";

  // check the room and find the user with nickname;
  map<string, cheat_room*>::iterator iter;
  if(server->map_rooms.find(room_name) == server->map_rooms.end()){
    // create the room and add a user.

    cheat_user* new_user = message->GetUser();
    cheat_room* new_cheat_room = new cheat_room(room_name);

    new_user->SetNickName(nick_name);
    new_user->EnterRoom(new_cheat_room);

    server->map_rooms[room_name] = new_cheat_room;

    json_ret["is_created"] = true;
    return message->GetUser()->SendMessageWithMain(json_ret.dump());
  }
  else{
    json_ret["is_created"] = false;
    json_ret["error_reason"] = "room_existed";
    return message->GetUser()->SendMessageWithMain(json_ret.dump());
  }
}


int jyc_cheat_server::HandleEnterTheRoomWithNickName(jyc_cheat_server* server, cheat_message* message){
  json json_recv = json::parse(message->GetMessage());
  string room_name = json_recv["room_name"].get<string>();
  string nick_name = json_recv["nickname"].get<string>();
  cheat_user* user_to_enter;
  cheat_room* room_to_enter;

  json json_ret;
  json_ret["res_tag"] = "enter_the_room_with_nickname";

  // check the room and find the user with nickname;
  map<string, cheat_room*>::iterator iter;
  cout << "finding with "<< room_name << endl;
  if(server->map_rooms.find(room_name) != server->map_rooms.end()){
    cout << "we found :"<< room_name << endl;
    room_to_enter = server->map_rooms[room_name];

    message->GetUser()->SetNickName(nick_name);
    // the room was existing.
    if(room_to_enter->IsExistUser(message->GetUser()) == 0){
      // the user was not existing.
      user_to_enter = message->GetUser();
      user_to_enter->SetNickName(nick_name);
      user_to_enter->EnterRoom(room_to_enter);
      
      json_ret["is_entering"] = true;
      return message->GetUser()->SendMessageWithMain(json_ret.dump());
    }
    else{
      json_ret["is_entering"] = false;
      json_ret["error_reason"] = "same_nick_name_existed";
      return message->GetUser()->SendMessageWithMain(json_ret.dump());
    }
  }

  json_ret["is_entering"] = false;
  json_ret["error_reason"] = "room_not_existed";
  return message->GetUser()->SendMessageWithMain(json_ret.dump());
}


int jyc_cheat_server::HandleSendCheatMessage(jyc_cheat_server* server, cheat_message* message){

  cheat_user* user = message->GetUser();
  cheat_room* cheat_room = user->GetCurrentCheatRoom();

  if(user && cheat_room){
    cheat_room->SendMessageToMembers(message);
  }
  else{
    cout << "In HandleSendCheatMessage() a user or cheat_room pointer variable is NULL." << endl;
  }

  return 0;
}


int jyc_cheat_server::AddAcceptedUser(){
  cout << "add new accepted user()." << endl;
  return 0;
}


int jyc_cheat_server::StartReceivingMessages(){
  pthread_create(&this->thread_check_disconnection, NULL, jyc_cheat_server::ReceiveMessagesLogic, (void*)this);
  return 0;
}


void *jyc_cheat_server::ReceiveMessagesLogic(void *t){
  string new_string = "test";

  jyc_cheat_server* server = (jyc_cheat_server*)*&t;
  
  while(true){
    usleep(100);
    list<cheat_user*>::iterator iter_user;
    for(iter_user = server->list_user.begin(); iter_user != server->list_user.end(); ++iter_user){
      string str_message = (*iter_user)->ReceiveMessage();
      if(str_message.compare("") != 0){
	cheat_message message((*iter_user), str_message);
	cout << "get message : " << message.GetMessage().c_str() << endl;
	server->queue_message.push(message);
      }
    }
  }
}


