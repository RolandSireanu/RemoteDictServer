#include <MkdirCommand.h>
#include <iostream>


// class MkdirHandler : public BaseHandler
// {
//     public:
//         bool Handle(std::string arg_request);
// };


void MkdirHandler::createPath(char* arg_path, const char* arg_startingPath)
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

bool MkdirHandler::Handle(std::string arg_request)
{
    if(arg_request.find_first_of("MKDIR") == 0)
    {
        // extractPathSize(arg_request)
        //createPath()
    }
    std::cout << "MkdirHandler not suitable, forward request ..." << std::endl;
    return BaseHandler::Handle(arg_request);
}