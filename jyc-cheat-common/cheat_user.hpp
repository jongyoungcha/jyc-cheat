#ifndef _CHEAT_USER_HPP
#define _CHEAT_USER_HPP

#include "common.hpp"
#include "cheat_room.hpp"

using namespace std;

namespace jyc_cheat
{
  class cheat_room;

  class cheat_user{
  private:
    cheat_room* current_room;
    string user_id;
    string nick_name;
    int socket;

  public:
    cheat_user(int sock);

    virtual ~cheat_user();

    int EnterRoom(cheat_room* room);

    int GetSocket();

    cheat_room* GetCurrentCheatRoom();

    int SetNickName(string nickname);

    string GetNickName();

    string ReceiveMessage();

    int SendMessage(string message);
  };
}

#endif


