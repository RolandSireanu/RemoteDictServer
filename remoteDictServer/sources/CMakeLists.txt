file(GLOB SRC_FILES    
"*.cpp"
)

message(${SRC_FILES} " ")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})