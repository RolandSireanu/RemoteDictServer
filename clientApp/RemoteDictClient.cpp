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

std::string server_ip{"127.0.0.1"};
constexpr uint32_t SERVER_PORT = 60000;


int main(int argc, char** argv)
{

    RemoteDictApi::configure(server_ip, SERVER_PORT);    
    std::cout << "Set request Bitcoin , 31451" << std::endl;
    RemoteDictApi::set("Bitcoin", "31451");
    std::cout << "Set request Ethereum , 1958" << std::endl;
    RemoteDictApi::set("Ethereum", "1958");
    std::cout << "Set request Ripple , 33" << std::endl;
    RemoteDictApi::set("Ripple", "33");    
    
    std::string r = RemoteDictApi::get("Bitcoin");            
    std::cout << "Get request Bitcoin: " << r << std::endl;
    r = RemoteDictApi::get("Ethereum");            
    std::cout << "Get request Ethereum: " << r << std::endl;
    r = RemoteDictApi::get("Ripple");            
    std::cout << "Get request Ripple: " << r << std::endl;
    std::cout << "Get stats report :" << std::endl;    
    RemoteDictApi::Statistics sts = RemoteDictApi::stats(); 
    std::cout << "\t nr of get requests: " << sts.getOperations << std::endl;
    std::cout << "\t nr of successful get requests: " << sts.successfulGetOperations << std::endl;
    std::cout << "\t nr of failed get requests: " << sts.failedGetOperations << std::endl;

    try
    {
        r = RemoteDictApi::get("Dollar");
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error catched " << e.what() << '\n';
    }

    return 0;
}