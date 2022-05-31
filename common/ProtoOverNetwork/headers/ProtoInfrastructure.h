#ifndef PROTO_INFRASTRUCTURE_H
#define PROTO_INFRASTRUCTURE_H

#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <thread>

namespace ProtoInfrastructure
{
    namespace 
    {
        //On socket arg_socket send arg_data number, representing the length of the following message
        bool sendMessageLength(int arg_socket, uint32_t arg_data)
        {
            uint32_t dataInNetworkFormat = htonl(arg_data);
            char* buffer =(char*) &dataInNetworkFormat;
            size_t lengthOfTheMessage = sizeof(arg_data);
            size_t bytesSentSoFar = 0;
            size_t result = 0;
            bool sentOk = true;

            do
            {                                        
                result = send(arg_socket, buffer + bytesSentSoFar, lengthOfTheMessage - bytesSentSoFar, MSG_NOSIGNAL);                
                if(result < 0)
                {
                    std::cout << "[Error] writing error ! " << std::endl;
                    sentOk = false;
                    break;
                }
                else
                {
                    bytesSentSoFar += result;
                }
                
            } while (bytesSentSoFar < lengthOfTheMessage);

            return sentOk;
        }

        size_t readMessageLength(int arg_socket)
        {
            uint32_t buffer;
            const size_t NR_OF_BYTES_TO_READ = sizeof(buffer);                        
            char* ptrResponse = (char*) &buffer;
            size_t bytesReadSoFar = 0;

            do
            {
                bytesReadSoFar += read(arg_socket, ptrResponse+bytesReadSoFar, NR_OF_BYTES_TO_READ - bytesReadSoFar);
            } while (bytesReadSoFar < NR_OF_BYTES_TO_READ);
            uint32_t receivedMessageLength = ntohl(buffer);

            return receivedMessageLength;
        }
    }

    extern std::mutex mutex_protection;

    template<typename T>
    bool sendProtocolBuffer(int arg_socket, const T& arg_protoBuffer)
    {
        char sendBuffer[512];
        bool ok = true;
        std::lock_guard<std::mutex> guard(mutex_protection);
        const size_t BYTES_TO_SEND = arg_protoBuffer.ByteSize();
        arg_protoBuffer.SerializeToArray(sendBuffer, BYTES_TO_SEND);
        size_t bytesSentSoFar = 0;
        size_t result = 0;

        ok = sendMessageLength(arg_socket, BYTES_TO_SEND);
        if(ok)
        {
            do
            {
                result = send(arg_socket, sendBuffer+bytesSentSoFar, BYTES_TO_SEND-bytesSentSoFar, MSG_NOSIGNAL);                
                if(result < 0)
                {
                    ok = false;
                    std::cout << "[Error] Data sending error " << std::endl;
                }
                else
                {
                    bytesSentSoFar += result;
                }

            } while (bytesSentSoFar < BYTES_TO_SEND);        
        }

        return ok;
    }

    template<typename T>
    T receiveProtoclBuffer(int arg_socket)
    {
        std::lock_guard<std::mutex> guard(mutex_protection);
        uint32_t nrOfBytesToRead = readMessageLength(arg_socket);

        size_t bytesReadSoFar = 0;               
        char* receiveBuffer = new char[nrOfBytesToRead];
        do
        {                                                    
            bytesReadSoFar += read(arg_socket, receiveBuffer+bytesReadSoFar, nrOfBytesToRead - bytesReadSoFar);            
        } while ((nrOfBytesToRead - bytesReadSoFar) > 0);        

        T result;
        result.ParseFromArray(receiveBuffer, nrOfBytesToRead);

        delete receiveBuffer;

        return result;
    }
};


#endif