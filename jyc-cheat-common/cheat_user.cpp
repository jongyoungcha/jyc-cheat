#include "cheat_user.hpp"

using namespace jyc_cheat;
cheat_user::cheat_user(int sock){
  this->socket = sock;
}

cheat_user::~cheat_user(){
  close(this->socket);
}

int cheat_user::EnterRoom(cheat_room* room){
  if(room){
    this->current_room = room;
  }

  return 0;
}

int cheat_user::GetSocket(){
  return this->socket;
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
      
  while((recv_size = recv(this->socket, &buf, 8192, 0)) > 0){
    sleep(1);
    message += buf;
    buf[0] = '\0';
  }

  return message;
}

int cheat_user::SendMessage(string message){
  int ret = 0;

  cout << "result message : " << message << endl;
  if((ret = write(this->socket, message.c_str(), strlen(message.c_str())+1)) == -1){
    cout << "When sended message, Get -1 of return value." << endl;
  }
       
  return ret;
}



