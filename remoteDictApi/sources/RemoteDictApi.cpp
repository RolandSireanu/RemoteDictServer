#include <RemoteDictApi.h>
#include <stdexcept>

namespace RemoteDictApi
{
    std::string ip;
    uint16_t port;
}

namespace 
{
    using s_sockAddr = struct sockaddr;

    class NewConnection
    {
        public:
            NewConnection(const char* arg_server_ip, uint16_t arg_server_port)
            {
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if(sockfd < 0)
                    throw std::runtime_error("Socket didn't get created !");

                memset(&serverAddr, 0, sizeof(serverAddr));
                serverAddr.sin_family = AF_INET;
                //The htons() function makes sure that numbers are stored in memory in network byte order, which is with the most significant byte first.
                serverAddr.sin_port = htons(arg_server_port);

                if(inet_pton(AF_INET, arg_server_ip, &serverAddr.sin_addr) <= 0)
                    throw std::runtime_error("inet_pton didn't reach a valid result ");

                if(connect(sockfd, (s_sockAddr*)&serverAddr, sizeof(serverAddr)) < 0)
                    throw std::runtime_error("Connection error");
            }

            int getSocket()
            {
                return sockfd;
            }

            ~NewConnection() { close(sockfd); }
        private:
            int sockfd;
            struct sockaddr_in serverAddr;


    };
}

struct sockaddr_in serverAddr;
constexpr uint16_t SERVER_PORT = 60000;
using SA = struct sockaddr;

int getNewConnection()
{
    int sockfd, n;    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        throw std::runtime_error("Socket didn't get created !");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    //The htons() function makes sure that numbers are stored in memory in network byte order, which is with the most significant byte first.
    serverAddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
        throw std::runtime_error("inet_pton didn't reach a valid result ");

    if(connect(sockfd, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Connection error");

    return sockfd;
}



void RemoteDictApi::configure(const std::string& arg_ip, uint32_t arg_port)
{
    std::cout << "RemoteDictApi::configure" << std::endl;
    ip = arg_ip;
    port = arg_port;
}

bool RemoteDictApi::set(const std::string& arg_k, const std::string& arg_v)
{
    std::cout << "RemoteDictApi::set called \n" << std::endl;
    NewConnection con(ip.c_str(), port);
    int sock = con.getSocket();
    communication::Request tempReq;
    communication::Request::SetRequest* tempSetReq = new communication::Request::SetRequest;
    tempSetReq->set_key(arg_k);
    tempSetReq->set_value(arg_v);
    tempReq.set_allocated_set(tempSetReq);    
    tempReq.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_SET_REQUEST);
    std::cout <<"Sending protocol buffer " << std::endl;
    ProtoInfrastructure::sendProtocolBuffer(sock, tempReq);
    std::cout <<"Protocol buffer sent " << std::endl;

    communication::Response ret = ProtoInfrastructure::receiveProtoclBuffer<communication::Response>(sock);
    if(ret.responsetype() == communication::Response::TypeOfResponse::Response_TypeOfResponse_SET_RESPONSE
        && ret.setresp().success() == true)
        return true;
    else
    {
        std::cout << ret.setresp().failure_message() << std::endl;
        return false;
    }
}

std::string RemoteDictApi::get(const std::string& arg_k)
{
    NewConnection con(ip.c_str(), port);
    int sock = con.getSocket();
    communication::Request tempReq;
    communication::Request::GetRequest* tempGetReq = new communication::Request::GetRequest;
    tempGetReq->set_key(arg_k);
    tempReq.set_allocated_get(tempGetReq);    
    tempReq.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_GET_REQUEST);
    ProtoInfrastructure::sendProtocolBuffer(sock, tempReq);

    communication::Response ret = ProtoInfrastructure::receiveProtoclBuffer<communication::Response>(sock);
    if(ret.responsetype() == communication::Response::TypeOfResponse::Response_TypeOfResponse_GET_RESPONSE
        && ret.getresp().success() == true)
        {            
            return ret.getresp().value();
        }
    else
    {        
        throw std::runtime_error(ret.getresp().value());        
    }
}

RemoteDictApi::Statistics RemoteDictApi::stats()
{
    RemoteDictApi::Statistics tempStats;
    NewConnection con(ip.c_str(), port);
    int sock = con.getSocket();
    communication::Request tempReq;            
    tempReq.set_requesttype(communication::Request::TypeOfRequest::Request_TypeOfRequest_STATS_REQUEST);
    ProtoInfrastructure::sendProtocolBuffer(sock, tempReq);    
    communication::Response ret = ProtoInfrastructure::receiveProtoclBuffer<communication::Response>(sock);
    if(ret.responsetype() == communication::Response::TypeOfResponse::Response_TypeOfResponse_STATS_RESPONSE)
    {
        tempStats.getOperations = ret.statresp().nr_of_get_operations();
        tempStats.failedGetOperations = ret.statresp().failed_get_operations();
        tempStats.successfulGetOperations = ret.statresp().successful_get_operations();
    }
    else
        throw std::runtime_error("[Error] Wrong response received !");

    return tempStats;

}