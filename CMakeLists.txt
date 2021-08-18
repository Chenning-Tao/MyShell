cmake_minimum_required(VERSION 3.15)
project(MyShell)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyShell main.cpp Head.h command.cpp command.h)
