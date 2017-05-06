#include <common.hpp>
#include <cheat_room.hpp>
#include <cheat_user.hpp>
#include <cheat_message.hpp>
#include "safe_queue.hpp"

using json=nlohmann::json;

namespace jyc_cheat
{
  typedef struct Feature{
    char shortcut;
    string description;
  }Feature;

  const int features_length = 3;
  const Feature features[features_length] = {
    {'p', " [ Print server list ] "},
    {'c', " [ Create the room ] "},
    {'e', " [ Enter the room ] "},
  };

  class jyc_cheat_client{
  private:
    // attributes
    int sock_client;
    int sock_async_cheat;
    string user_input; 
    struct sockaddr_in clientaddr;
    struct sockaddr_in clientaddr_cheat;
    safe_queue<json> queue_message;
    list<cheat_user> list_users;
    list<cheat_room> list_rooms;

    bool is_alive_message_receiver;
    pthread_t thread_message_receiver;

    int win_menu_width;
    int win_menu_height;
    WINDOW* win_menu;
    WINDOW* win_room_list;
    WINDOW* win_user_input;
    WINDOW* win_cheat_messages_box;
    WINDOW* win_cheat_users_box;

    // the class logics
    int ConnectServer();
    int StartLogic();
    int ShowIntro();
    int SelectMainFeature(Feature* feature_selected);
    int InformListMode(string title, list<string> list_item);
    int CheatingMode(const string room_name, const string nick_name);
    int ManageRoomMode(bool is_creating, string* room_name, string* nick_name);
    int CheatRoomLogic(string room_name, string nick_name);

    // thread logics
    void StartReceiveCheatMessagesThroughBackground();
    static void *ReceiveCheatMessagesThroughBackground(void* t);
    void StopReceiverCheatMessage();

    // the protocols
    int SendMessageToRoomMembers(string message);
    int GetRoomList(list<string>* proom_list);
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

