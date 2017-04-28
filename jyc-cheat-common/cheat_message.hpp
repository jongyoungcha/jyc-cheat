#ifndef _CHEAT_MESSAGE_HPP_
#define _CHEAT_MESSAGE_HPP_

#include "common.hpp"
#include "cheat_user.hpp"

using namespace std;

namespace jyc_cheat{

  class cheat_user;

  class cheat_message{
  private:
    cheat_user* user;
    string message;

  public:
    cheat_message(cheat_user* user, string message);
    virtual ~cheat_message();
    cheat_user* GetUser();
    string GetMessage();
  };
}


#endif


