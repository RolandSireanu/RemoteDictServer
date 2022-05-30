file(GLOB SRC_FILES    
"*.cc"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_FILES})