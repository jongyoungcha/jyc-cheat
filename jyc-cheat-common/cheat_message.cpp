#include "cheat_message.hpp"

using namespace jyc_cheat;
using namespace std;

cheat_message::cheat_message(cheat_user* user, string message){
  this->user = user;
  this->message = message;
}

cheat_message::~cheat_message(){}

cheat_user* cheat_message::GetUser(){
  return this->user;
}

string cheat_message::GetMessage(){
  return this->message;
}
 
