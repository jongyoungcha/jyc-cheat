cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project (jyc_cheat_client)
subdirs (../jyc-cheat-common)
include_directories(../jyc-cheat-common)
add_executable(jyc_cheat_client main.cpp jyc_cheat_client.cpp)

target_link_libraries(jyc_cheat_client jyc_cheat_common)
