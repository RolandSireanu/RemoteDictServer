cmake_minimum_required(VERSION 3.1.0)
project(GenProto_project)
set(CMAKE_BUILD_TYPE Release)


add_custom_target(genProto ALL 
    COMMAND protoc -I=../ --cpp_out=../ ../communication.proto
)   