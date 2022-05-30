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
        std::cout << "Send " << bytesSentSoFar << " bytes over socket " << std::endl;        
    } while (bytesSentSoFar < lengthOfTheMessage);
}


void HandleConnection::sendData(int arg_socket, char* arg_data, size_t arg_lenght)
{
    size_t bytesSentSoFar = 0;
    do
    {
        std::cout << "I will send " << arg_lenght - bytesSentSoFar << " data bytes over socket arg_lenght = " << arg_lenght << "\n";               
        bytesSentSoFar += write(arg_socket, arg_data + bytesSentSoFar, arg_lenght - bytesSentSoFar);
        std::cout << "Send " << bytesSentSoFar << " data bytes over socket " << std::endl;        
    } while (bytesSentSoFar < arg_lenght);
}



void HandleConnection::operator()(TSQueue& arg_queue, RemoteDictionary& arg_rmtDict)
{
    size_t bytes_read = 0;   
    char sendBuffer[MAXLINE];
    char receiveBuffer[MAXLINE];
    int n = 0;
    
    do
    {                
        int connFd = arg_queue.pop();
        if(connFd != -1)
        {
            std::cout << "Popped item in thread " << connFd << std::endl;
            size_t bytesReadSoFar = 0;
            //Reset data            
            memset(sendBuffer, 0, MAXLINE);
            memset(receiveBuffer, 0, MAXLINE);
            n = 0;
            
            std::cout << "Receiving protocol buffer \n";
            communication::Request request = ProtoInfrastructure::receiveProtoclBuffer<communication::Request>(connFd);
            std::cout << "Protocol buffer received ! " << std::endl;

            communication::Response resp;
            switch (request.requesttype())
            {
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_SET_REQUEST:
                    std::cout << " Set request received \n";
                    resp = arg_rmtDict.set(request.set().key(), request.set().value());
                    //ProtoInfrastructure::sendProtocolBuffer(connFd, resp);
                    break;
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_GET_REQUEST:                
                    std::cout << " Get request received \n";
                    resp = arg_rmtDict.get(request.get().key());
                    //ProtoInfrastructure::sendProtocolBuffer(connFd, resp);
                    break;
                case communication::Request_TypeOfRequest::Request_TypeOfRequest_STATS_REQUEST:
                    resp = arg_rmtDict.stats();                                                                                                                     
                    std::cout << " Stats request received \n";
                    break;
            
                default: 
                    std::cout << "[Error] Unknown request received !" << std::endl;                   
                    break;
            }
            ProtoInfrastructure::sendProtocolBuffer(connFd, resp);
            close(connFd);
        }

        //sleep 10 ms
        usleep(10000);
    } while(1); 
}