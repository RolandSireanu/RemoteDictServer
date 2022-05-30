file(GLOB SRC_FILES    
"*.cpp"
)

message(${SRC_FILES} " ")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_FILES})