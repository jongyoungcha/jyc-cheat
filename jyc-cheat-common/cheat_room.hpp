#ifndef _CHEAT_ROOM_HPP_
#define _CHEAT_ROOM_HPP_

#include "common.hpp"
#include "cheat_user.hpp"
#include "cheat_message.hpp"
#include "json.hpp"

using namespace std;

namespace jyc_cheat
{
  class cheat_message;
  class cheat_user;
  class cheat_room{
  private:
    string room_name;
    map<string, cheat_user*> map_users;

  public:
    cheat_room(string name);

    virtual ~cheat_room();

    int IsExistUser(cheat_user* user_to_find);

    int AddUser(cheat_user* added_user);

    int RemoveUser(cheat_user* user_to_remove);

    int GetCurrentRoomMembers(list<cheat_user>* user_list);

    int SendMessageToMembers(cheat_message* message);
  };
}

#endif
