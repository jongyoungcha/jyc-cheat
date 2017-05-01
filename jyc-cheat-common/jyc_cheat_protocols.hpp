#ifndef _JYC_CHEAT_PROTOCOLS_
#define _JYC_CHEAT_PROTOCOLS_

#include <common.hpp>
#include "json.hpp"
#include "cheat_room.hpp"
#include "cheat_user.hpp"
#include "cheat_message.hpp"

using namespace std;
using json=nlohmann::json;

namespace jyc_cheat{
  static const int err_code = 0;
  enum prtc_code_res{
    prtc_code_res_success= 2000,
    prtc_code_res_error,
  };

  enum prtc_code_req{
    prtc_code_req_get_room_list=1000,
    prtc_code_req_enter_the_room,
    prtc_code_req_exit_the_room,
    prtc_code_req_register_nick_name,
    prtc_code_req_send_message_to_room_members,
  };


  class protocol{
  private:
  public:
    protocol(){}
    virtual ~protocol(){}
    virtual string ToXml() = 0;
  };

  class prtc_get_room_list : public protocol{
  private:
    json j;

  public:
    prtc_get_room_list(){
      j["req_tag"] = "get_room_list";
    }
    
    virtual string ToXml(){
      return j.dump();
    }
  };


  class prtc_is_existing_the_room : public protocol{
  private:
    json j;
  public:
    prtc_is_existing_the_room(string room_name){
      j["req_tag"] = "is_existing_the_room";
      j["room_name"] = room_name;
    }

    virtual string ToXml(){
      return j.dump();
    }
  };

  class prtc_create_the_room_with_nickname : public protocol{
  private:
    json j;
  public:
    prtc_create_the_room_with_nickname(string room_name, string nickname){
      j["req_tag"] = "create_the_room_with_nickname";
      j["room_name"] = room_name;
      j["nickname"] = nickname;
    }

    virtual string ToXml(){
      return j.dump();
    }
  };

  class prtc_enter_the_room_with_nickname : public protocol{
  private:
    json j;
  public:
    prtc_enter_the_room_with_nickname(string room_name, string nickname){
      j["req_tag"] = "enter_the_room_with_nickname";
      j["room_name"] = room_name;
      j["nickname"] = nickname;
    }

    virtual string ToXml(){
      return j.dump();
    }
  };

  class prtc_send_cheat_message : public protocol{
  private:
    json j;
  public:
    prtc_send_cheat_message(string room_name, string nickname, string message){
      j["req_tag"] = "send_cheat_message";
      j["room_name"] = room_name;
      j["nickname"] = nickname;
      j["message"] = message;
    }

    virtual string ToXml(){
      return j.dump();
    }
  };
}


#endif

