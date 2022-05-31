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
#include <stdexcept>
#include <fstream>
#include <thread>
#include "ThreadSafeQueue.h"
#include <stdexcept>
#include <string>
#include <HandleConnection.h>
#include <RemoteDictionary.h>
#ifdef EnableTests
    #include<gtest/gtest.h> 
#endif

constexpr unsigned int SERVER_PORT = 60000;
constexpr unsigned int  MAXLINE = 4096;
constexpr size_t NR_OF_THREADS = 8;
using SocketAddress = struct sockaddr;
TSQueue tsQueue;
RemoteDictionary rmtDict;

int main(int ac, char** av)
{

    #ifdef EnableTests
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
    #else

        int listenfd, connfd, n;
        struct sockaddr_in serverAddr;
        std::thread threads[NR_OF_THREADS];        
        HandleConnection handleConnection;

        for(int i=0; i<NR_OF_THREADS; i++)
        {
            threads[i] = std::thread(handleConnection, std::ref(tsQueue), std::ref(rmtDict));
            threads[i].detach();
        }
        
        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)    
            throw std::runtime_error("Socket instantiation failed! ");

        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(SERVER_PORT);

        if((bind(listenfd, (SocketAddress*) &serverAddr, sizeof(serverAddr))) < 0)
            throw std::runtime_error("Bind failed ");

        if((listen(listenfd,10)) < 0)
            throw std::runtime_error("Listen error ");

        while(1)
        {
            try
            {
                struct sockaddr_in addr;
                socklen_t addr_len;

                std::cout << "Waiting for a connection on port " << SERVER_PORT << std::endl;
                fflush(stdout);

                int connfd = accept(listenfd, (SocketAddress*)&addr, &addr_len);                        
                tsQueue.push(connfd);   
            }
            catch (std::exception const& e) 
            {
                std::cout << "Exception in main thread " << e.what() << std::endl;
            } 
        }        
    return 0;
    #endif
}