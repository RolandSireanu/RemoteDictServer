#include <HandleConnection.h>
#include <ProtoInfrastructure.h>
#include <arpa/inet.h>

void HandleConnection::sendMessageLength(int arg_socket, uint32_t arg_data)
{
    uint32_t dataInNetworkFormat = htonl(arg_data);
    char* buffer =(char*) &dataInNetworkFormat;
    size_t lengthOfTheMessage = sizeof(arg_data);
    size_t bytesSentSoFar = 0;

    do
    {                        
        bytesSentSoFar += write(arg_socket, buffer + bytesSentSoFar, lengthOfTheMessage - bytesSentSoFar);        
    } while (bytesSentSoFar < lengthOfTheMessage);
}


void HandleConnection::sendData(int arg_socket, char* arg_data, size_t arg_lenght)
{
    size_t bytesSentSoFar = 0;
    do
    {        
        bytesSentSoFar += write(arg_socket, arg_data + bytesSentSoFar, arg_lenght - bytesSentSoFar);        
    } while (bytesSentSoFar < arg_lenght);
}



void HandleConnection::operator()(TSQueue& arg_queue, RemoteDictionary& arg_rmtDict)
{        
    do
    {                
        int connFd = arg_queue.pop();
        if(connFd != -1)
        {                        
            //Reset data                                                 
            communication::Request request = ProtoInfrastructure::receiveProtoclBuffer<communication::Request>(connFd);
            communication::Response resp;
            switch (request.requesttype())
            {
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_SET_REQUEST:                    
                    resp = arg_rmtDict.set(request.set().key(), request.set().value());                    
                    break;
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_GET_REQUEST:                                    
                    resp = arg_rmtDict.get(request.get().key());                    
                    break;
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_STATS_REQUEST:
                    resp = arg_rmtDict.stats();                                                                                                                                         
                    break;
            
                default: 
                    std::cout << "[Error] Unknown request received !" << std::endl;                   
                    break;
            }
            ProtoInfrastructure::sendProtocolBuffer(connFd, resp);
            close(connFd);
        }

        // TODO: replace this with condition variable 
        //sleep 10 ms
        usleep(1000);
    } while(1); 
}