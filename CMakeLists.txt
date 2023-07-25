cmake_minimum_required(VERSION 3.25)
project(ECS_Test)

set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ECS)
include_directories(${PROJECT_SOURCE_DIR}/PackedArray)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(COMPILER_FLAGS -g -Wall -pedantic -Wextra)
endif ()
if (CMAKE_BUILD_TYPE MATCHES Release)
    set(COMPILER_FLAGS -O2)
endif ()

set (ECS_SOURCE ECS/ComponentBase.h
                ECS/ComponentManager.h
                ECS/ECS.h
                ECS/Entity.h
                ECS/EntityManager.h
                ECS/ObjectManager.h
                ECS/System.h
                ECS/SystemManager.h
                ECS/Types.h)

set(CONTAINER_SOURCE PackedArray/PackedArray.h)

set(TEST_SOURCE main_test.cpp
                Test/TestComponents.h
                Test/TestSystems.h)

set (EXAMPLE_SOURCE main_example.cpp
                    Example/EntityAction.h
                    Example/EntityActionManager.h
                    Example/EntityActionSystem.h
                    Example/EntityActionSystemComponentRemover.h
                    Example/EntityActionSystemSwitcher.h
                    Example/Object.h
                    Example/PrivateEntityAction.h
                    Example/PublicEntityAction.h)

add_executable(example ${EXAMPLE_SOURCE} ${CONTAINER_SOURCE} ${ECS_SOURCE} )

target_compile_options ( example PRIVATE ${COMPILER_FLAGS} )

add_executable(tests ${TEST_SOURCE} ${CONTAINER_SOURCE} ${ECS_SOURCE} )