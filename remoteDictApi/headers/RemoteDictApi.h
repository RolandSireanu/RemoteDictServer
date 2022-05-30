#ifndef REMOTE_DICT_API_H
#define REMOTE_DICT_API_H
#include <string>
#include <communication.pb.h>
#include <ProtoInfrastructure.h>

namespace RemoteDictApi
{
    struct Statistics
    {
        size_t getOperations;
        size_t successfulGetOperations;
        size_t failedGetOperations;
    };

    extern std::string ip;
    extern uint16_t port;

    bool set(const std::string& arg_k, const std::string& value);
    std::string get(const std::string& arg_k);
    Statistics stats();
    void configure(const std::string& arg_ip, uint32_t arg_port);
};


#endif