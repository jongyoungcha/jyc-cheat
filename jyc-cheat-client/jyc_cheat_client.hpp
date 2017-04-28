#include <common.hpp>
#include <cheat_room.hpp>
#include <cheat_user.hpp>
#include <cheat_message.hpp>

namespace jyc_cheat
{
  class jyc_cheat_client{
  private:
    int sock_client;
    string user_input; 
    struct sockaddr_in clientaddr;
    list<cheat_message> queue_messages;

    list<cheat_user> list_users;
    list<cheat_room> list_rooms;

    // the class logics
    int ConnectServer();
    int StartLogic();
    int CheatRoomLogic(string room_name, string nick_name);
    int PrintMenu();

    // the protocols
    int SendMessageToRoomMembers(string message);
    int GetRoomList();
    int IsExistingTheRoom(string room_name);
    int CreateTheRoomWithNickName(string room_name, string nickname);
    int EnterTheRoomWithNickName(string room_name, string nickname);
    int ExitTheRoom(string room_id);

  public:
    jyc_cheat_client();
    ~jyc_cheat_client();
    int StartClient();
  };
}

