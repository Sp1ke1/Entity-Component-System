cmake_minimum_required(VERSION 3.25)
project(ECS_Test)

set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ecs)
include_directories(${PROJECT_SOURCE_DIR}/PackedArray)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(COMPILER_FLAGS -g -Wall -pedantic -Wextra)
endif ()
if (CMAKE_BUILD_TYPE MATCHES Release)
    set(COMPILER_FLAGS -O2 -g)
endif ()

add_compile_options(${COMPILER_FLAGS})

set(ECS_SOURCE ecs/ComponentManager.h ecs/ComponentManager.cpp
        ecs/ECS.h ecs/ECS.cpp
        ecs/Entity.h ecs/Entity.cpp
        ecs/EntityManager.h ecs/EntityManager.cpp
        ecs/System.h ecs/System.cpp
        ecs/SystemManager.h ecs/SystemManager.cpp
        ecs/ObjectManager.h
        ecs/Types.h
        ecs/ECSInclude.h
        ecs/Components/ComponentBase.cpp ecs/Components/ComponentBase.h)

set(CONTAINER_SOURCE PackedArray/PackedArray.h PackedArray/PackedArray.cpp)

set(TEST_SOURCE EntityAction.h EntityAction.cpp
        EntityActionManager.h EntityActionManager.cpp
        main.cpp
        Object.h ecs/ECSInclude.h
        ecs/Systems/ExampleSystems.h
        ecs/Components/ExampleComponents.h)

add_executable(ECS_TEST ${ECS_SOURCE} ${CONTAINER_SOURCE} ${TEST_SOURCE})
