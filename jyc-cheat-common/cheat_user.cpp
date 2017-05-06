#include "cheat_user.hpp"

using namespace jyc_cheat;
cheat_user::cheat_user(int sock){
  this->sock_main = sock;
}

cheat_user::~cheat_user(){
  close(this->sock_main);
}

int cheat_user::EnterRoom(cheat_room* room){
  if(room){
    this->current_room = room;
    room->AddUser(this);
  }

  return 0;
}

int cheat_user::GetMainSock(){
  return this->sock_main;
}

int cheat_user::SetSubAsyncSock(int sock){
  int flags = 0;
  
  flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  return this->sock_async_sub = sock;
}

int cheat_user::GetSubAsyncSock(){
  return this->sock_async_sub;
}

cheat_room* cheat_user::GetCurrentCheatRoom(){
  return this->current_room;
}

int cheat_user::SetNickName(string nickname){
  this->nick_name = nickname;
  return 0;
}

string cheat_user::GetNickName(){
  return this->nick_name;
}

string cheat_user::ReceiveMessage(){
  string message;
  char buf[8192] = {0};
  int recv_size = 0;
      
  while((recv_size = recv(this->sock_main, &buf, 8192, 0)) > 0){
    usleep(100);
    message += buf;
    buf[0] = '\0';
  }

  return message;
}

int cheat_user::SendMessageWithMain(string message){
  int ret = 0;

  cout << "send message with main : " << message << endl;
  if((ret = write(this->sock_main, message.c_str(), strlen(message.c_str())+1)) == -1){
    cout << "When sended message, Get -1 of return value." << endl;
  }
       
  return ret;
}

int cheat_user::SendMessageWithAsyncSub(string message){
  int ret = 0;

  cout << "send messge with sub : " << message << endl;
  if((ret = write(this->sock_async_sub, message.c_str(), strlen(message.c_str())+1)) == -1){
    cout << "When sended message, Get -1 of return value." << endl;
  }
       
  return ret;
}


