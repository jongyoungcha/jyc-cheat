#include <common.hpp>
#include "jyc_cheat_client.hpp"
#include <json.hpp>
#include <jyc_log.h>

using json = nlohmann::json;
using namespace jyc_cheat;
using namespace std;

int main(int argc, char* argv[]){
  
  jyc_log_init("./", "jyc_cheat_client", JYC_LOG_DEBUG);
  jyc_cheat_client client;
  client.StartClient();
  
  return 0;
}


