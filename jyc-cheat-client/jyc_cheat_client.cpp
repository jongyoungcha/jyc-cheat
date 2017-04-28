#include "jyc_cheat_client.hpp"
#include <jyc_cheat_protocols.hpp>

using namespace std;
using namespace jyc_cheat;

jyc_cheat_client::jyc_cheat_client(){
  this->sock_client = socket(AF_INET, SOCK_STREAM, 0);
  this->clientaddr.sin_family = AF_INET;
  this->clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  this->clientaddr.sin_port = htons(9999);
}

jyc_cheat_client::~jyc_cheat_client(){
}


int jyc_cheat_client::StartClient(){
  
  int clientaddr_len = sizeof(this->clientaddr);
  
  if (connect(this->sock_client, (struct sockaddr *)&this->clientaddr, clientaddr_len) < 0){
    perror("Connect error: ");
    exit(0);
  }
  this->StartLogic();

  return 0;
}


int jyc_cheat_client::PrintMenu(){
  printf(":::::::::::::::::::::::::::::::::::\n");
  printf(":::: Jongyoungcha's Cheat Menu ::::\n");
  printf(":::::::::::::::::::::::::::::::::::\n");
  printf("- <p> Print server list.\n");
  printf("- <c> Create the room.\n");
  printf("- <e> Enter the room.\n");
  printf("\n");
  printf("** Please input user command ** : ");
  return 0;
}

int jyc_cheat_client::StartLogic(){
  
  string room_name;
  string nick_name;
  int ret = 0;
  while(true){
    
    this->PrintMenu();
   
    getline(cin, this->user_input);

    if (user_input.length() == 1) {
      char menu_shortcut = user_input[0];
      switch(menu_shortcut){
      case 'p':
	this->GetRoomList();
	break;
      case 'c':
	this->GetRoomList();
	cout << "Please input the room name you want to create : ";
	getline(cin, room_name);

	ret = this->IsExistingTheRoom(room_name);

	if (ret == 1){
	  // maked.
	  cout << "Please input user nick name : " ;
	  getline(cin, nick_name);
	  this->CreateTheRoomWithNickName(room_name, nick_name);
	  this->CheatRoomLogic(room_name, nick_name);
	}
	else if (ret == 0){
	  // not maked.
	  cout << "Sorry, The room name that inputted was existing...." ;
	}
	else{
	  //error
	  cout << "Error occured......" ;
	}
	break;
      case 'e':
	this->GetRoomList();
	cout << "Please input the room name you want to enter : ";
	getline(cin, room_name);
	ret = this->IsExistingTheRoom(room_name);

	if (ret == 1){
	  // maked.
	  cout << "Please input user nick name : " ;
	  getline(cin, nick_name);
	  ret = this->EnterTheRoomWithNickName(nick_name, room_name);
	  this->CheatRoomLogic(room_name, nick_name);
	}
      default:
	cout << "Your input was not matched with anything else." << endl;
	break;
      }
    }
    else{
      cout << "You inputted not one charater. Please input one thing." << endl;
    }
    cout << endl;
  }

  return 0;
}


int jyc_cheat_client::CheatRoomLogic(string room_name, string nick_name){
  string message_inputted;
  string str_to_send_server;
  int read_len;
  char buf[8192] = {0};

  json json_received;
  string str_received;
  string res_tag;
  string message_received;
  string send_user;

  while(true){
    cout << "*** please input your messgae : ";
    getline(cin, message_inputted);

    // make 
    prtc_send_cheat_message send_cheat_message(room_name, nick_name, message_inputted);
    str_to_send_server = send_cheat_message.ToXml();
    if(write(this->sock_client, str_to_send_server.c_str(), str_to_send_server.size()) == -1){
      cout << "error while write()" << endl;
      return -1;
    }

    read_len = read(this->sock_client, buf, sizeof(buf));
    str_received = buf;
    json_received = json::parse(str_received);

    if((res_tag = json_received["res_tag"].get<string>()).compare("send_cheat_message") == 0){
      send_user = json_received["send_user"].get<string>(); 
      message_received = json_received["message"].get<string>(); 

      cout << send_user << " : " << message_received << endl;
    } 
  }
}


int jyc_cheat_client::ExitTheRoom(string room_id){
  return 0;
}


int jyc_cheat_client::SendMessageToRoomMembers(string message){
  return 0;
}

int jyc_cheat_client::GetRoomList(){
  char buf[8192] = {0};
  string message;
  int read_len;
  cout << "Called GetRoomList()" << endl;

  if(this->sock_client){

    prtc_get_room_list get_room_list;
    message = get_room_list.ToXml();
    cout << "send message : " << message << endl;

    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      cout << "error while write()" << endl;
      return -1;
    }

    read_len = read(this->sock_client, buf, sizeof(buf));
    printf("recv message : %s\n", buf);
    
    // Handling section readed json data.
    json recv_json = json::parse(buf); 
    cout << "<< the room list >>" << endl;
    for(json::iterator iter = recv_json["rooms"].begin(); iter != recv_json["rooms"].end(); ++iter){
      cout << iter->get<std::string>() << '\n';
    }
  }
  return 0;
}


int jyc_cheat_client::IsExistingTheRoom(string room_name){
  cout << "Called MakeTheRoom()" << endl;
  char buf[8192] = {0};
  string message;
  int read_len;
  if(this->sock_client){
    prtc_is_existing_the_room is_existing_the_room(room_name);
    message = is_existing_the_room.ToXml();
    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      return -1;
    }
    read_len = read(this->sock_client, buf, sizeof(buf));
    printf("recv message : %s\n", buf);

    json recv_json = json::parse(buf);
    cout << "received and parsed json : " << recv_json << endl;
    bool is_exist = recv_json["is_exist"];
    if(is_exist){
      // not maked the room.
      return 0;
    }
    else{
      // maked the room.
      return 1;
    }
  }
  
  // error
  cout << "socket was closed." << '\n';
  return -1;
}


int jyc_cheat_client::CreateTheRoomWithNickName(string room_name, string nickname){

  cout << "Called CreateTheRoomWithNickName()" << endl;
  char buf[8192] = {0};
  string message;
  int read_len;

  if(this->sock_client){
    prtc_create_the_room_with_nickname enter_the_room_with_nickname(room_name, nickname);
    message = enter_the_room_with_nickname.ToXml();
    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      return -1;
    }
    read_len = read(this->sock_client, buf, sizeof(buf));
    printf("recv message : %s\n", buf);
  }

  return 0;
}


int jyc_cheat_client::EnterTheRoomWithNickName(string room_name, string nickname){
  cout << "Called EnterTheRoomWithNickName()" << endl;
  char buf[8192] = {0};
  string message;
  int read_len;
  if(this->sock_client){
    prtc_enter_the_room_with_nickname enter_the_room_with_nickname(room_name, nickname);
    message = enter_the_room_with_nickname.ToXml();
    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      return -1;
    }
    read_len = read(this->sock_client, buf, sizeof(buf));
    printf("recv message : %s\n", buf);
  }
  return 0;
}






            









