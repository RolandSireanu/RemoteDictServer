#ifndef HANDLE_CONNECTION_H
#define HANDLE_CONNECTION_H

#include "ThreadSafeQueue.h"
#include <string.h>
#include <unistd.h>
#include <communication.pb.h>
#include <RemoteDictionary.h>
#include <ProtoInfrastructure.h>

class HandleConnection
{

    public:
        void operator()(TSQueue& arg_queue, RemoteDictionary& arg_rmtDict);
    private:
        void sendMessageLength(int arg_socket, uint32_t arg_data);


        void sendData(int arg_socket, char* arg_data, size_t arg_lenght);
        const size_t MAXLINE = 512;
};

#endif