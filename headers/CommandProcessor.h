#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include <string>


class Handler
{
    public:
        virtual Handler* SetNext(Handler* arg_handler) = 0;
        virtual bool Handle(std::string arg_request) = 0;
};

class BaseHandler : public Handler 
{
    public:
        BaseHandler();
        virtual Handler* SetNext(Handler* arg_handler);
        virtual bool Handle(std::string arg_request);

    private:
        Handler* nextHandler;
};



#endif