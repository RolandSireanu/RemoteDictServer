#ifndef PUT_COMMAND_H
#define PUT_COMMAND_H

#include <CommandProcessor.h>

class PutHandler : public BaseHandler
{
    public:
        virtual bool Handle(std::string arg_request);
};

#endif