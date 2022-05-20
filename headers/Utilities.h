#include <cstdio>
#include <string.h>
#include <filesystem>
#include <iostream>

std::size_t extractPathSize(const char* arg_req, std::size_t arg_startPos);
void createPath(char* arg_path, const char* arg_startingPath);