#ifndef MKDIR_COMMAND_H
#define MKDIR_COMMAND_H

#include <CommandProcessor.h>
#include <filesystem>
#include <Utilities.h>

class MkdirHandler : public BaseHandler
{
    public:
        virtual bool Handle(std::string arg_request);
    private:
        void createPath(char* arg_path, const char* arg_startingPath);
};

#endif