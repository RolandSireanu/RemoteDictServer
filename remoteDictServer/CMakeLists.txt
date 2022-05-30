cmake_minimum_required(VERSION 3.1.0)
project(FTS)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

if(EnableTests)
    message("========= GTests enabled ! ===========")
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
    add_definitions(-DEnableTests)
    add_subdirectory(headers)
    add_subdirectory(sources)
    add_subdirectory(tests)
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        gtest_main
        gtest
        )
    
    include(GoogleTest)
    gtest_discover_tests(${CMAKE_PROJECT_NAME})
endif()




set(PROJECT_NAME RemoteDictionaryServer)
add_executable(${PROJECT_NAME} ${SOURCES})
add_subdirectory(headers)
add_subdirectory(sources)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoBuffers)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoOverNetwork/headers)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoBuffers/communication.pb.cc)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoOverNetwork/sources/ProtoInfrastructure.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
target_link_libraries(${PROJECT_NAME} PRIVATE protobuf)



