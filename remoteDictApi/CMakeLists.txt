cmake_minimum_required(VERSION 3.1.0)
project(RemoteDictApi_project)
set(CMAKE_BUILD_TYPE Release)

set(REMOTE_DICT_API RemoteDictApi)
set(CMAKE_BUILD_TYPE Release)
# set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")


add_library(${REMOTE_DICT_API} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/RemoteDictApi.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoBuffers/communication.pb.cc

)
target_link_libraries(${REMOTE_DICT_API} PUBLIC protobuf)

target_include_directories(${REMOTE_DICT_API} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_include_directories(${REMOTE_DICT_API} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoBuffers)
target_include_directories(${REMOTE_DICT_API} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoOverNetwork/headers)
target_sources(${REMOTE_DICT_API} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ProtoOverNetwork/sources/ProtoInfrastructure.cpp)
# set_target_properties(${REMOTE_DICT_API} PROPERTIES PUBLIC_HEADER "headers/RemoteDictApi.h;../common/ProtoBuffers/communication.pb.h;../common/ProtoOverNetwork/headers/ProtoInfrastructure.h")
# install(TARGETS ${REMOTE_DICT_API} 
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include)