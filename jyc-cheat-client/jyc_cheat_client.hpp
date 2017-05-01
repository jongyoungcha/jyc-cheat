#include <common.hpp>
#include <cheat_room.hpp>
#include <cheat_user.hpp>
#include <cheat_message.hpp>
#include "safe_queue.hpp"

using json=nlohmann::json;

namespace jyc_cheat
{
  class jyc_cheat_client{
  private:
    int sock_client;
    string user_input; 
    struct sockaddr_in clientaddr;
    safe_queue<json> queue_message;
    list<cheat_user> list_users;
    list<cheat_room> list_rooms;

    bool is_alive_message_receiver;
    pthread_t thread_message_receiver;

    // the class logics
    int ConnectServer();
    int StartLogic();
    int CheatRoomLogic(string room_name, string nick_name);
    int PrintMenu();

    // thread logics
    void StartReceiveCheatMessagesThroughBackground();
    static void *ReceiveCheatMessagesThroughBackground(void* t);
    void StopReceiverCheatMessage();

    // the protocols
    int SendMessageToRoomMembers(string message);
    int GetRoomList();
    int IsExistingTheRoom(string room_name);
    int CreateTheRoomWithNickName(string room_name, string nickname, string* error_reason);
    int EnterTheRoomWithNickName(string room_name, string nickname, string* error_reason);
    int ExitTheRoom(string room_id);

  public:
    jyc_cheat_client();
    ~jyc_cheat_client();
    int StartClient();
  };
}

