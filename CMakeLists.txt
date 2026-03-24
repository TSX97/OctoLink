cmake_minimum_required(VERSION 3.5)
project(ink)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
include_directories(include)

add_executable(ink_server
        src/server.cpp
        src/client.cpp
        src/protocol.cpp
        src/tools.cpp
) #сервер

add_executable(ink_client
        src/client_app.cpp
        src/protocol.cpp
        src/tools.cpp
)

target_link_libraries(ink_server Boost::system nlohmann_json::nlohmann_json  pthread)
target_link_libraries(ink_client Boost::system nlohmann_json::nlohmann_json  pthread)