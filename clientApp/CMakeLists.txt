cmake_minimum_required(VERSION 3.1.0)
project(ClientApplication)
set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/RemoteDictClient.cpp)

# Server API
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../remoteDictApi/lib)
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC RemoteDictApi)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../remoteDictApi/headers)