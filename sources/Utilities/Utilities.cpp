#include "Utilities.h"

std::size_t extractPathSize(const char* arg_req, std::size_t arg_startPos)
{
    std::size_t i = arg_startPos;
    std::size_t sizeOfPath = 0;
    while(arg_req[i] >= '0' && arg_req[i] <= '9')
    {
        sizeOfPath = (sizeOfPath*10) + (arg_req[i] - 48);
        i++;
    }

    return sizeOfPath > 0 ? sizeOfPath : -1;
}


void createPath(char* arg_path, const char* arg_startingPath)
{
    std::string startingPath {arg_startingPath};
    std::string finalPath {startingPath + "/" + std::string{arg_path}};

    std::size_t position = finalPath.find("//");    
    while(position != std::string::npos)
    {
        finalPath.erase(position,1);
        position = finalPath.find("//");
    }

    if(std::filesystem::exists(finalPath) == false)
    {
        std::filesystem::create_directories(finalPath);    
    }
    
}