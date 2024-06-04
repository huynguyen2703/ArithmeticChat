cmake_minimum_required(VERSION 3.27)
project(ClientServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(ClientServer server/server.cpp
        client/client.cpp
        ../../Downloads/lab1/lab1.cpp)
