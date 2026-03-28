cmake_minimum_required(VERSION 3.20)
project(ink)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)

include_directories(include)

# Server
add_executable(ink_server
        src/main.cpp
        src/server.cpp
        src/client.cpp
        src/manager.cpp
        src/fishman.cpp
        src/tools.cpp
)

# Client
add_executable(ink_client
        src/client_app.cpp
        src/client.cpp
        src/manager.cpp
        src/tools.cpp
        src/protocol.cpp
)

target_link_libraries(ink_server
        Boost::system
        nlohmann_json::nlohmann_json
        pthread
)

target_link_libraries(ink_client
        Boost::system
        nlohmann_json::nlohmann_json
        pthread
)