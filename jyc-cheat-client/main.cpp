#include <common.hpp>
#include "jyc_cheat_client.hpp"
#include <json.hpp>

using json = nlohmann::json;
using namespace jyc_cheat;
using namespace std;

int main(int argc, char* argv[]){
  printf("Hello jyc-cheat-client!!\n");
  jyc_cheat_client client;
  client.StartClient();
  return 0;
}
