#include <CommandProcessor.h>

BaseHandler::BaseHandler():nextHandler(nullptr) 
{
    
}

Handler* BaseHandler::SetNext(Handler* arg_handler)
{
    this->nextHandler = arg_handler;
    return arg_handler;
}

bool BaseHandler::Handle(std::string arg_request)
{
    if(this->nextHandler != nullptr)
        return this->nextHandler->Handle(arg_request);

    return true;
}
