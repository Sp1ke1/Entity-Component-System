cmake_minimum_required(VERSION 3.25)
project(ECS_Test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ECS_Test main.cpp ObjectManager.cpp EntityActionManager.h EntityAction.h EntityActionManager.cpp Entity.h Object.cpp Entity.cpp EntityAction.cpp)
