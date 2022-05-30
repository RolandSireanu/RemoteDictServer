#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <RemoteDictApi.h>
#include <communication.pb.h>

// constexpr unsigned int SERVER_PORT = 90001;
constexpr unsigned int  MAXLINE = 4096;
std::string server_ip{"127.0.0.1"};
constexpr uint32_t SERVER_PORT = 60000;
using SA = struct sockaddr;


// void sendMessageLength(int arg_socket, uint32_t arg_data)
// {
//     uint32_t dataInNetworkFormat = htonl(arg_data);
//     char* buffer =(char*) &dataInNetworkFormat;
//     size_t lengthOfTheMessage = sizeof(arg_data);
//     size_t bytesSentSoFar = 0;

//     do
//     {                        
//         bytesSentSoFar += write(arg_socket, buffer + bytesSentSoFar, lengthOfTheMessage - bytesSentSoFar);            
//         std::cout << "Send " << bytesSentSoFar << " bytes over socket " << std::endl;        
//     } while (bytesSentSoFar < lengthOfTheMessage);
// }


// void sendData(int arg_socket, char* arg_data, size_t arg_lenght)
// {
//     size_t bytesSentSoFar = 0;
//     do
//     {                 
//         std::cout << "I will send " << arg_lenght - bytesSentSoFar << " data bytes over socket arg_lenght = " << arg_lenght << "\n";               
//         bytesSentSoFar += write(arg_socket, arg_data + bytesSentSoFar, arg_lenght - bytesSentSoFar);
//         std::cout << "Send " << bytesSentSoFar << " data bytes over socket " << std::endl;        
//     } while (bytesSentSoFar < arg_lenght);
// }

// bool sendDataOverSocket(int arg_socket, char* arg_data, size_t arg_length)
// {

//     std::cout << "Size of arg_data = " << arg_length << std::endl;
//     sendMessageLength(arg_socket, arg_length);
//     sendData(arg_socket, arg_data, arg_length);




//     // bytesSentSoFar = 0;
//     // messageLength = sizeof(arg_data);    
//     // do
//     // {                        
//     //     std::cout << "Send " << messageLength - bytesSentSoFar << " bytes over socket " << std::endl;
//     //     bytesSentSoFar += write(arg_socket, arg_data + bytesSentSoFar, messageLength - bytesSentSoFar);
//     //     sleep(1);
//     // } while (bytesSentSoFar < messageLength);

//     // std::cout << "Send " << BYTES_TO_SEND << " nr of bytes !" << std::endl;
//     //     bytesSendSoFar += write(sockfd, &sendLine[bytesSendSoFar], BYTES_TO_SEND);
//     //     std::cout << "Bytes sent so far " << bytesSendSoFar << std::endl;
// }
struct sockaddr_in serverAddr;
int getNewConnection(char** arg_argv)
{
    int sockfd, n;    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        throw std::runtime_error("Socket didn't get created !");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    //The htons() function makes sure that numbers are stored in memory in network byte order, which is with the most significant byte first.
    serverAddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, arg_argv[1], &serverAddr.sin_addr) <= 0)
        throw std::runtime_error("inet_pton didn't reach a valid result ");

    if(connect(sockfd, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Connection error");

    return sockfd;
}


int main(int argc, char** argv)
{
    // int sockfd, n;    
    // struct sockaddr_in serverAddr;
    // char sendLine[MAXLINE];
    // char recvLine[MAXLINE];

    RemoteDictApi::configure(server_ip, SERVER_PORT);
    std::cout << "Config done " << std::endl;
    RemoteDictApi::set("S22Ultra", "4300");
    RemoteDictApi::set("S21Plus", "3750");
    RemoteDictApi::set("S20", "3100");
    std::cout << "Set done " << std::endl;
    std::string r = RemoteDictApi::get("S22Ultra");        
    std::cout << r << std::endl;
    r = RemoteDictApi::get("S20");        
    std::cout << r << std::endl;
    r = RemoteDictApi::get("S21Plus");        
    std::cout << r << std::endl;    
    RemoteDictApi::Statistics s = RemoteDictApi::stats();
    std::cout << s.getOperations << std::endl;
    try
    {
        r = RemoteDictApi::get("S21PlusExtra");
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error catched " << e.what() << '\n';
    }
    
    

    // if (argc < 2)
    //     throw "Please provide the destination IP address in cli ";

    // // protocol = 0 = TCP
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if(sockfd < 0)
    //     throw std::runtime_error("Socket didn't get created !");

    // memset(&serverAddr, 0, sizeof(serverAddr));
    // serverAddr.sin_family = AF_INET;
    // //The htons() function makes sure that numbers are stored in memory in network byte order, which is with the most significant byte first.
    // serverAddr.sin_port = htons(SERVER_PORT);

    // if(inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0)
    //     throw std::runtime_error("inet_pton didn't reach a valid result ");

    // if(connect(sockfd, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
    //     throw std::runtime_error("Connection error");    

    
    // communication::Request req;
    // communication::Request::SetRequest setRequest;
    // =====================================================
    // int sockfd = getNewConnection(argv);    
    // communication::Request request;
    // communication::Request::SetRequest setReq;
    // setReq.set_key("Key_A");
    // setReq.set_value("Value_1");
    // request.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_SET_REQUEST);
    // request.set_allocated_set(&setReq);
    // ProtoInfrastructure::sendProtocolBuffer(sockfd, request);
    // communication::Response setResponse = ProtoInfrastructure::receiveProtoclBuffer<communication::Response>(sockfd);
    // ======================================================
    // int sockfd2 = getNewConnection(argv);
    // communication::Request request_2;
    // communication::Request::GetRequest getReq;
    // getReq.set_key("Key_A");
    // request_2.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_GET_REQUEST);
    // request_2.set_allocated_get(&getReq);
    // ProtoInfrastructure::sendProtocolBuffer(sockfd2, request_2);
    // communication::Response getResponse = ProtoInfrastructure::receiveProtoclBuffer<communication::Response>(sockfd2);
    // if(getResponse.responsetype() == communication::Response::TypeOfResponse::Response_TypeOfResponse_GET_RESPONSE)
    //     std::cout << "Get(Key_A) = " << getResponse.getresp().value();
    // else
    //     std::cout << "Unknown response received " << std::endl;

    // int sockfd3 = getNewConnection(argv);
    // communication::Request request_3;
    // request_3.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_STATS_REQUEST);
    // ProtoInfrastructure::sendProtocolBuffer(sockfd3, request_3);

    // ======================================================================================= 

    // request.SET_REQUEST.set_key("MyKey1");
    // setRequest.set_key("MyKey1");
    // setRequest.set_value("MyValue1");
    
    //communication::SetResponse setResp = ProtoInfrastructure::receiveProtoclBuffer<communication::SetResponse>(sockfd);
    //std::cout << "setResp.success " << setResp.success() << std::endl;


    // const size_t BYTES_TO_SEND = setRequest.ByteSize();
    // std::cout << "BYTES_TO_SEND = " << BYTES_TO_SEND << std::endl;
    // setRequest.SerializeToArray(sendLine, BYTES_TO_SEND);
    // sendDataOverSocket(sockfd, sendLine, BYTES_TO_SEND);
    

    // do
    // {
    //     uint32_t messageLength = htonl(BYTES_TO_SEND);
    //     char* buffer = (char*)&messageLength;
    //     size_t tempNrOfBytes = write(sockfd, buffer, sizeof(messageLength));
    //     if(tempNrOfBytes != sizeof(messageLength))
    //         std::cout << "[ERROR] Message length sending error ! " << std::endl;

    //     std::cout << "Send " << BYTES_TO_SEND << " nr of bytes !" << std::endl;
    //     bytesSendSoFar += write(sockfd, &sendLine[bytesSendSoFar], BYTES_TO_SEND);
    //     std::cout << "Bytes sent so far " << bytesSendSoFar << std::endl;
    // } while (bytesSendSoFar < BYTES_TO_SEND);
    
    // if(write(sockfd, sendLine, bytesToSend) != sendBytes)
    //     throw std::runtime_error("Communication error ");


    // memset(recvLine, 0, MAXLINE);
    // do
    // {
    //     n = read(sockfd, recvLine, MAXLINE-1);
    //     std::cout << recvLine << std::endl;
    // } while (n > 0);
    
    exit(0);

    return 0;
}