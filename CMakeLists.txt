cmake_minimum_required(VERSION 3.25)
project(ECS_Test)

set(CMAKE_CXX_STANDARD 17)

include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/ecs )
include_directories(${PROJECT_SOURCE_DIR}/PackedArray)
include_directories( ${PROJECT_SOURCE_DIR}/)

set ( ECS_SOURCE ecs/ComponentManager.h ecs/ComponentManager.cpp
        ecs/ECS.h ecs/ECS.cpp
                 ecs/Entity.h ecs/Entity.cpp
                 ecs/EntityManager.h ecs/EntityManager.cpp
                 ecs/ExampleComponents.h
                 ecs/System.h ecs/System.cpp
                 ecs/SystemManager.h ecs/SystemManager.cpp
                 ecs/ObjectManager.h
        ecs/Types.h ecs/Component.cpp ecs/Component.h ecs/ECSInclude.h)

set ( CONTAINER_SOURCE PackedArray/PackedArray.h PackedArray/PackedArray.cpp)

set ( TEST_SOURCE EntityAction.h EntityAction.cpp
                  EntityActionManager.h EntityActionManager.cpp
                  main.cpp
                  Object.h ecs/Component.cpp ecs/Component.h ecs/ECSInclude.h)

add_executable( ECS_TEST ${ECS_SOURCE} ${CONTAINER_SOURCE} ${TEST_SOURCE} )
