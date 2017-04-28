#include <common.hpp>
#include <cheat_user.hpp>
#include <cheat_room.hpp>
#include <cheat_message.hpp>
#include <safe_queue.hpp>
#include <wqueue.h>
#include <json.hpp>

using namespace jyc_cheat;

namespace jyc_cheat
{
  class jyc_cheat_server{

  private:
    int server_sock;
    fd_set rfds;
    struct sockaddr_in server_addr;
    list<cheat_user*> list_user;
    map<string, cheat_room*> map_rooms;
    pthread_t thread_check_disconnection;
    pthread_t thread_message_consummer;
    safe_queue<cheat_message> queue_message;

  public:
    jyc_cheat_server();
    ~jyc_cheat_server();
    string CurrentServerState();
    int MakeTestData();
    int StartServer();
    int StartListen();
    int StartAccept();
    int AddAcceptedUser();
    int RemoveUser(cheat_user* user);

    int StartReceivingMessages();
    static void *ReceiveMessagesLogic(void* t);
    int HandleRoomList(jyc_cheat_server* server, cheat_message* user);
    int HandleIsExistingTheRoom(jyc_cheat_server* server, cheat_message* user);
    int HandleCreateTheRoomWithNickName(jyc_cheat_server* server, cheat_message* user);
    int HandleEnterTheRoomWithNickName(jyc_cheat_server* server, cheat_message* user);
    int HandleSendCheatMessage(jyc_cheat_server* server, cheat_message* user);

    int MessageHandler(jyc_cheat_server* server, cheat_message* message);
    int SendCuurentRoomList();

    int StartConsumeMessages();
    static void *ConsumeMessagesLogic(void* t);
  };
}
  

