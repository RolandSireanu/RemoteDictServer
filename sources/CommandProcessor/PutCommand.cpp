#include <PutCommand.h>
#include <iostream>

bool PutHandler::Handle(std::string arg_request)
{
    std::cout << "PutHandler not suitable, forward request ..." << std::endl;
    return BaseHandler::Handle(arg_request);    
}