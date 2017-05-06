#include "jyc_cheat_client.hpp"
#include <jyc_cheat_protocols.hpp>
#include <jyc_log.h>

using namespace std;
using namespace jyc_cheat;

jyc_cheat_client::jyc_cheat_client(){

  this->sock_client = socket(AF_INET, SOCK_STREAM, 0);
  this->clientaddr.sin_family = AF_INET;
  this->clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  this->clientaddr.sin_port = htons(9999);

  this->win_menu_width = 50;
  this->win_menu_height = 11;

  initscr();
}

jyc_cheat_client::~jyc_cheat_client(){
}


int jyc_cheat_client::StartClient(){
  jyc_log_write(JYC_LOG_NORMAL, "Start Client!!");
  // this->ShowIntro();
  int clientaddr_len = sizeof(this->clientaddr);
  
  if (connect(this->sock_client, (struct sockaddr *)&this->clientaddr, clientaddr_len) == -1){
    jyc_log_write(JYC_LOG_ERROR, "connect faild....");
    exit(0);
  }
  this->StartLogic();

  return 0;
}


int jyc_cheat_client::ShowIntro(){
  initscr();
  int height, width, start_y, start_x;
  height = 10;
  width = 42;
  start_y = start_x = 4;
  WINDOW* intro_win = newwin(height, width, start_y, start_x);

  refresh();

  box(intro_win, 0, 0);
  mvwprintw(intro_win, 1, 2, ":::::::::::::::::::::::::::::::::::::");
  mvwprintw(intro_win, 2, 2, ":: JYC Cheat Client                ::");
  mvwprintw(intro_win, 3, 2, ":: The first writer : jongyoungcha ::");
  mvwprintw(intro_win, 4, 2, ":::::::::::::::::::::::::::::::::::::");
  wrefresh(intro_win);

  int c = getch();

  if(intro_win){
    free(intro_win);
  }
  endwin();
  clear();

  return 0;
}

int jyc_cheat_client::SelectMainFeature(Feature* feature_selected){

  this->win_menu = newwin(this->win_menu_height, this->win_menu_width, 1, 1);
  keypad(this->win_menu, true);

  int choice = 0;
  int highlight = 0;

  while(1){
    box(this->win_menu, 0, 0);
    mvwprintw(this->win_menu, 1, 2, ":::::::::::::::::::::::::::::::::::::");
    mvwprintw(this->win_menu, 2, 2, ":::   Jongyoungcha's Cheat Menu   :::");
    mvwprintw(this->win_menu, 3, 2, ":::::::::::::::::::::::::::::::::::::");

    for(int i = 0; i < 3; i++){
      if(i == highlight)
  	wattron(this->win_menu, A_REVERSE);
      mvwprintw(this->win_menu, features_length+i+1, 1, features[i].description.c_str());
      wattroff(this->win_menu, A_REVERSE);
    }

    refresh();
    wrefresh(this->win_menu);

    choice = wgetch(this->win_menu);
    switch(choice){

    case KEY_UP:
      highlight--;
      if(highlight == -1)
      	highlight = 0; 
      break;

    case KEY_DOWN:
      highlight++;
      if(highlight == features_length)
      	highlight = 2;
      break;

    case '\n':
      *feature_selected = features[highlight];
      delwin(this->win_menu);
      refresh();
      clear();
      endwin();
      return 0;
      
    default:
      break;
    }
  }
  
  return 0;
}

int jyc_cheat_client::StartLogic(){
  
  string room_name;
  string nick_name;
  string err_message;
  char shortcut_selected;
  Feature feature_selected;
  list<string> list_room;

  jyc_log_write(JYC_LOG_DEBUG, "StartLogic()");

  while(true){
    
    this->SelectMainFeature(&feature_selected);
    shortcut_selected = feature_selected.shortcut;

    switch(shortcut_selected){
    case 'p':
      this->GetRoomList(&list_room);
      this->InformListMode("<< Room List >>", list_room);
      list_room.clear();
      break;
    case 'c':
      if(this->ManageRoomMode(true, &room_name, &nick_name) != 1) { break; }
      this->CheatingMode(room_name, nick_name); 
      // this->CheatingMode("cheat_room4", "tester"); 
      break;
    case 'e':
      if(this->ManageRoomMode(false, &room_name, &nick_name) != 1) { break; }
      this->CheatingMode(room_name, nick_name); 
      break;
	  
    default:
      // cout << "Your input was not matched with anything else." << endl;
      break;
    }
  }

  return 0;
}

int jyc_cheat_client::InformListMode(string title, list<string> const list_items){

  int height, width, start_x, start_y;

  clear();
  refresh();
  height = 20;
  width = 60;
  start_x = 1;
  start_y = 1;

  this->win_room_list = newwin(height, width, start_x, start_y);
  box(win_room_list, 0, 0);
  mvwprintw(this->win_room_list, 1, 1, title.c_str());

  int current_line = 1;
  list<string>::const_iterator iter;
  for(iter = list_items.begin(); iter != list_items.end(); ++iter){
    mvwprintw(this->win_room_list, ++current_line, 1, iter->c_str());
  }

  wrefresh(this->win_room_list);
  refresh();
  
  getch();

  // delwin(this->win_room_list);
  clear();
  refresh();
  endwin();

  return 0;
}
  
int jyc_cheat_client::CheatingMode(const string room_name, const string nick_name){

  WINDOW* win_cheat_users_box = NULL;
  WINDOW* win_cheat_messages_border_box = NULL;
  WINDOW* win_cheat_messages_document_box = NULL;
  WINDOW* win_input_box = NULL;
  int win_doc_box_width = 80;
  int win_doc_box_height = 20;
  char message_inputted[8192] = {0};
  string str_to_send_server;

  // ncurses window setting.
  clear();
  refresh();

  win_cheat_messages_border_box = newwin(win_doc_box_height + 4, win_doc_box_width + 2, 1, 1);
  box(win_cheat_messages_border_box, 0, 0);
  mvwprintw(win_cheat_messages_border_box, 1, 1, "[ Cheat Messages ]");

  win_cheat_messages_document_box = newwin(win_doc_box_height, win_doc_box_width, 4, 2);
  // box(win_cheat_messages_document_box, 0, 0);
  scrollok(win_cheat_messages_document_box, true);
  scroll(win_cheat_messages_document_box);

  win_cheat_users_box = newwin(24, 25, 1, 85);
  box(win_cheat_users_box, 0, 0);
  mvwprintw(win_cheat_users_box, 1, 1, "[ Cheat Users ]");
  // scrollok(win_cheat_users_box, true);
  // scroll(win_cheat_users_box);

  wrefresh(win_cheat_messages_border_box);
  wrefresh(win_cheat_messages_document_box);
  wrefresh(win_cheat_users_box);

  this->win_cheat_messages_box = win_cheat_messages_document_box;
  this->win_cheat_users_box = win_cheat_users_box;

  win_input_box = newwin(4, 105, 25, 1);

  this->StartReceiveCheatMessagesThroughBackground();

  while(1){
    wclear(win_input_box);
    box(win_input_box, 0, 0);
    mvwprintw(win_input_box, 1, 1, "[ Input Your Message ]");
    mvwprintw(win_input_box, 2, 1, "%s : ", nick_name.c_str());

    wrefresh(win_input_box);

    wmove(win_input_box, 2, 3 + strlen(nick_name.c_str()) + 1);

    wgetnstr(win_input_box, message_inputted, 8192-1);
    mvwprintw(win_input_box, 2, 1, message_inputted);

    wrefresh(win_input_box);
    if(strcmp(message_inputted, "quit()") == 0){
      this->StopReceiverCheatMessage();
      refresh();
      clear();
      break;
    }

    // make 
    prtc_send_cheat_message send_cheat_message(room_name, nick_name, message_inputted);
    str_to_send_server = send_cheat_message.ToXml();
    if(write(this->sock_client, str_to_send_server.c_str(), str_to_send_server.size()) == -1){
      mvwprintw(win_input_box, 2, 1, "Error while send a message.");
      jyc_log_write(JYC_LOG_ERROR, "Error while send a message.");
      return -1;
    }
  }

  if(win_cheat_messages_border_box){
    delwin(win_cheat_messages_border_box);
    this->win_cheat_messages_box = NULL;
  }

  if(win_cheat_users_box){
    delwin(win_cheat_users_box);
    this->win_cheat_users_box = NULL;
  }

  if(win_input_box) delwin(win_input_box);

  refresh();
  clear();

  return 0;
}

// When is_creating is true : creating the room.
// When is_creating is false : entering the room.
int jyc_cheat_client::ManageRoomMode(bool is_creating, string* room_name, string* nick_name){
  
  WINDOW* win_current_rooms = NULL;
  WINDOW* win_info_box = NULL;
  WINDOW* win_create_room = NULL;
  int ret = 0;
  int current_line = 1;
  char room_inputted[8192] = {0};
  char nickname_inputted[8192] = {0};
  string err_msg;
  list<string> list_current_rooms;
  
  clear();
  
  // Create current room list box.
  this->GetRoomList(&list_current_rooms);

  win_current_rooms = newwin(17, 50, 1, 1);
  box(win_current_rooms, 0, 0);
  mvwprintw(win_current_rooms, 1, 1, "[ Registered Room List ]");

  list<string>::iterator iter;
  for(iter = list_current_rooms.begin(); iter != list_current_rooms.end(); ++iter){
    mvwprintw(win_current_rooms, ++current_line, 1, iter->c_str());
  }

  // Create info box 
  win_info_box = newwin(3, 50, 18, 1);
  box(win_info_box, 0, 0);

  win_create_room = newwin(5, 50, 21, 1);
  box(win_create_room, 0, 0);

  wrefresh(win_current_rooms);
  wrefresh(win_info_box);
  wrefresh(win_create_room);
  
  // Check the room was existing.
  while(1){
    wclear(win_info_box);
    box(win_info_box, 0, 0);
    if(is_creating){
      mvwprintw(win_info_box, 1, 1, "Please input the room name want to make.");
    }
    else{
      mvwprintw(win_info_box, 1, 1, "Please input the room name want to enter.");
    }
    wrefresh(win_info_box);

    wclear(win_create_room);
    box(win_create_room, 0, 0);
    mvwprintw(win_create_room, 1, 1, ": ");
    wmove(win_create_room, 1, 3);
    wrefresh(win_create_room);

    wgetnstr(win_create_room, room_inputted, 8192-1);
    mvwprintw(win_create_room, 2, 1, room_inputted);

    wclear(win_info_box);
    box(win_info_box, 0, 0);
    ret = this->IsExistingTheRoom(room_inputted);
    if(is_creating){
      if ((ret = this->IsExistingTheRoom(room_inputted)) == 1){
	// When you are creating new room and It was not existing.
	mvwprintw(win_info_box, 1, 1, "The room \"%s\" was existing.", room_inputted);
      }
      else{
	break;
      }
    }
    else{
      if ((ret = this->IsExistingTheRoom(room_inputted)) == 0){
	//When you are entering new room and It was existing.
	mvwprintw(win_info_box, 1, 1, "The room \"%s\" was not existing.", room_inputted);
      }
      else{
	break;
      }
    }
    wrefresh(win_info_box);
    getch();
			 
  }


  // Inputing nick name
  if(is_creating){
    // When you are creating...
    while(1){
      wclear(win_info_box);
      box(win_info_box, 0, 0);
      mvwprintw(win_info_box, 1, 1, "Please input your nickname.");
      wrefresh(win_info_box);

      wclear(win_create_room);
      box(win_create_room, 0, 0);
      mvwprintw(win_create_room, 1, 1, ": ");
      wmove(win_create_room, 1, 3);
      wrefresh(win_create_room);

      wgetnstr(win_create_room, nickname_inputted, 8192-1);
      mvwprintw(win_create_room, 2, 1, nickname_inputted);

      if(strlen(nickname_inputted) > 4){
	break;
      }
    }

    this->CreateTheRoomWithNickName(room_inputted, nickname_inputted, &err_msg);
    *room_name = room_inputted;
    *nick_name = nickname_inputted;
  }
  else{
    // When you are entering...
    while(1){
      wclear(win_info_box);
      box(win_info_box, 0, 0);
      mvwprintw(win_info_box, 1, 1, "Please input your nickname.");
      wrefresh(win_info_box);

      wclear(win_create_room);
      box(win_create_room, 0, 0);
      mvwprintw(win_create_room, 1, 1, ": ");
      wmove(win_create_room, 1, 3);
      wrefresh(win_create_room);

      wgetnstr(win_create_room, nickname_inputted, 8192-1);
      mvwprintw(win_create_room, 2, 1, nickname_inputted);
      ret = this->EnterTheRoomWithNickName(room_inputted, nickname_inputted, &err_msg);
      if(ret == 1){
	*room_name = room_inputted;
	*nick_name = nickname_inputted;
	break;
      }
      else if(ret == 0){
	wclear(win_info_box);
	box(win_info_box, 0, 0);
	mvwprintw(win_info_box, 1, 1, "The nickname you inputted was existing.");
	wrefresh(win_info_box);

	getch();

	continue;
      }
      else{
	jyc_log_write(JYC_LOG_ERROR, "Error occured when enter the room.");
	return -1;
      }
    }
  }


  if (win_info_box) delwin(win_info_box);
  if (win_create_room) delwin(win_current_rooms);
  if (win_info_box) delwin(win_info_box);
  
  clear();
  refresh();
  endwin();

  return 1;
}


void jyc_cheat_client::StartReceiveCheatMessagesThroughBackground(){
  this->is_alive_message_receiver = true;
  pthread_create(&(this->thread_message_receiver), NULL, this->ReceiveCheatMessagesThroughBackground, (void*)this);
}


void *jyc_cheat_client::ReceiveCheatMessagesThroughBackground(void* t){
  
  jyc_cheat_client* client = (jyc_cheat_client*)t;

  int read_len = 0;
  string str_received;
  json json_received;
  char buf[8129] = {0};
  string res_tag;
  string send_user;
  string message_received;
  fd_set readfds;
  int state;

  // flags = fcntl(client->sock_client , F_GETFL, 0);
  // fcntl(client->sock_client, F_SETFL, flags | O_NONBLOCK);
  // cout << "connected client : " << client->sock_client << endl;


  while(client->is_alive_message_receiver &&
	client->win_cheat_messages_box &&
	client->win_cheat_users_box){

    FD_ZERO(&readfds);
    FD_SET(client->sock_client, &readfds);

    state = select(client->sock_client + 1, &readfds, NULL, NULL, NULL);
    if(state == -1){
      jyc_log_write(JYC_LOG_ERROR, "select() failed... in the cheating logic.");
      continue;
    }
    else{
      while((read_len = read(client->sock_client, buf, sizeof(buf))) > 0){
	str_received = buf;
	jyc_log_write(JYC_LOG_DEBUG, "received cheat message : %s", str_received.c_str());
    
	if(str_received.compare("") != 0){
	  json_received = json::parse(str_received);

	  if((res_tag = json_received["res_tag"].get<string>()).compare("send_cheat_message") == 0){
	    send_user = json_received["send_user"].get<string>(); 
	    message_received = json_received["message"].get<string>(); 

	    wprintw(client->win_cheat_messages_box,"%s : %s\n", send_user.c_str(), message_received.c_str());
	    wrefresh(client->win_cheat_messages_box);
	  } 
	}
      }
    }
  }

  return 0;
}


void jyc_cheat_client::StopReceiverCheatMessage(){
  jyc_log_write(JYC_LOG_DEBUG, "StopReceiverCheatMessage()");
  this->is_alive_message_receiver = false;
}


int jyc_cheat_client::ExitTheRoom(string room_id){
  return 0;
}


int jyc_cheat_client::SendMessageToRoomMembers(string message){
  return 0;
}

int jyc_cheat_client::GetRoomList(list<string>* proom_list){
  char buf[8192] = {0};
  string message;
  int read_len;

  if(this->sock_client){

    prtc_get_room_list get_room_list;
    message = get_room_list.ToXml();
    jyc_log_write(JYC_LOG_DEBUG, "GetRoomList request : %s", message.c_str());

    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      jyc_log_write(JYC_LOG_ERROR, "error occured while write()");
      return -1;
    }

    read_len = read(this->sock_client, buf, sizeof(buf));
    jyc_log_write(JYC_LOG_DEBUG, "GetRoomList response : %s", buf);

    // Handling section readed json data.
    json recv_json = json::parse(buf); 
    jyc_log_write(JYC_LOG_DEBUG, "<<room list>>");
    for(json::iterator iter = recv_json["rooms"].begin(); iter != recv_json["rooms"].end(); ++iter){
      jyc_log_write(JYC_LOG_DEBUG, iter->get<std::string>().c_str());
      proom_list->push_back(iter->get<std::string>());
    }
  }
  
  return 0;
}


int jyc_cheat_client::IsExistingTheRoom(string room_name){
  jyc_log_write(JYC_LOG_DEBUG, "Called MakeTheRoom()");
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
    jyc_log_write(JYC_LOG_DEBUG, "received json : %s", buf);

    json recv_json = json::parse(buf);
    bool is_exist = recv_json["is_exist"];
    if(is_exist){
      // the room was existing.
      return 1;
    }
    else{
      // the room was not existing.
      return 0;
    }
  }
  
  // error
  cout << "socket was closed." << '\n';
  return -1;
}


int jyc_cheat_client::CreateTheRoomWithNickName(string room_name, string nickname, string* err_message){

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


int jyc_cheat_client::EnterTheRoomWithNickName(string room_name, string nickname, string* err_message){
  cout << "Called EnterTheRoomWithNickName()" << endl;
  char buf[8192] = {0};
  string message;
  int read_len;
  json recv_json;
  bool is_entering;
  if(this->sock_client){
    prtc_enter_the_room_with_nickname enter_the_room_with_nickname(room_name, nickname);
    message = enter_the_room_with_nickname.ToXml();
    if(write(this->sock_client, message.c_str(), message.size()) == -1){
      return -1;
    }
    read_len = read(this->sock_client, buf, sizeof(buf));

    try{
      recv_json = json::parse(buf);
    }
    catch(int){
      // json parse failed..
      return -1;
    }

    is_entering = recv_json["is_entering"];
    if(is_entering == false){
      // entering the room was failed..
      *err_message = recv_json["error_reason"];
      return 0;
    }
    else{
      // entering the room was successed.
      return 1;
    }
    printf("recv message : %s\n", buf);
  }

  // socket was not opened.
  return -1;
}













