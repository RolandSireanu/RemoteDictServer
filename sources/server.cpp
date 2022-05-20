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
#include <ThreadSafeDeque.h>
#include <stdexcept>
#include <filesystem>
#include <Utilities.h>
#include <CommandProcessor.h>
#include <MkdirCommand.h>
#include <PutCommand.h>
#ifdef EnableTests
    #include<gtest/gtest.h> 
#endif

constexpr unsigned int SERVER_PORT = 40000;
constexpr unsigned int  MAXLINE = 4096;
constexpr size_t NR_OF_THREADS = 16;
using SA = struct sockaddr;
TSDeque<int> tsDeque;


void processRequest(char* arg_req, std::size_t length)
{
    constexpr std::size_t MAX_PATH_LENGTH = 1024;
    std::size_t sizeOfPath = 0;
    std::cout << arg_req << std::endl;
    if(strncmp("GET", arg_req, 3) == 0)
    {
        std::cout << "It is a GET request and path size is " << sizeOfPath << std::endl;
        std::size_t pathSize = extractPathSize(arg_req, 3);

    }
    else if(strncmp("PUT", arg_req, 3) == 0)
    {
        std::cout << "It is a PUT request " << std::endl;
        std::size_t pathSize = extractPathSize(arg_req, 3);
        char path[MAX_PATH_LENGTH];
        strncpy(path, arg_req+5, pathSize);
        std::ofstream outFile(path, std::ios::out | std::ios::binary);
        outFile << "Hello world";
        outFile.close();
        std::cout << path << std::endl;
    }
    else if(strncmp("MKDIR", arg_req, 5) == 0)
    {
        std::cout << "It is a make dir request " << std::endl;
        std::size_t pathSize = extractPathSize(arg_req, 5);
        char path[MAX_PATH_LENGTH];
        strncpy(path, arg_req+5+2, pathSize);        
        createPath(path, "/tmp");        
    }
    else
        throw std::runtime_error("Undefined action request !");
}

void HandleConnection()
{
    char recvline[MAXLINE+1];
    size_t bytes_read = 0;
    socklen_t addr_len;
    char buff[MAXLINE+1];
    char bigDataBuffer[MAXLINE*16];
    int n = 0;

    do
    {
        std::shared_ptr<int> connFd = tsDeque.pop();
        if(connFd != nullptr)
        {
            //Reset data
            memset(recvline, 0 , MAXLINE);
            memset(buff, 0, MAXLINE);
            memset(bigDataBuffer, 0 , MAXLINE*16);
            n = 0;

            std::thread::id this_id = std::this_thread::get_id();
            std::cout << "Thread " << this_id << " will process current request ! " << std::endl;
            std::ofstream out("Request");

            do
            {                
                std::cout << "reading from fd "<< *connFd << "..." << std::endl;
                bytes_read = read(*connFd, &bigDataBuffer[n], MAXLINE-1);
                std::cout << "bytes_read = " << bytes_read << std::endl;
                n = n + bytes_read;
                std::cout << "Total number of bytes read so far = " << n << std::endl;
                if(bigDataBuffer[n-1] == '\n')
                    break;
            } while (bytes_read > 0);

            out << bigDataBuffer;
            out.close();

        if (bytes_read < 0)
            throw std::runtime_error("Read error !");

        processRequest(bigDataBuffer, n);
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");   
        write(*connFd, (char*)buff, strlen((char*)buff));
        close(*connFd);
        }
    } while(1);    
}


int main(int ac, char** av)
{

    #ifdef EnableTests
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
    #else
        
        MkdirHandler* mkdirHandler = new MkdirHandler{};
        PutHandler* putHandler = new PutHandler{};

        mkdirHandler->SetNext(putHandler);
        mkdirHandler->Handle("Hello world !");

        int listenfd, connfd, n;
        struct sockaddr_in serverAddr;
        std::thread threads[NR_OF_THREADS];
        int thCounter = 0;
        
        for(int i=0; i<NR_OF_THREADS; i++)
        {
            threads[i] = std::thread(HandleConnection);
        }

        
        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)    
            throw std::runtime_error("Socket creation failed ");

        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(SERVER_PORT);

        if((bind(listenfd, (SA*) &serverAddr, sizeof(serverAddr))) < 0)
            throw std::runtime_error("Bind failed ");

        if((listen(listenfd,10)) < 0)
            throw std::runtime_error("Listen error ");

        while(1)
        {
            struct sockaddr_in addr;
            socklen_t addr_len;

            // std::cout << "Waiting for a connection on port " << SERVER_PORT << std::endl;
            fflush(stdout);

            connfd = accept(listenfd, (SA*)&addr, &addr_len);
            tsDeque.push(connfd);
            // threads[thCounter++] = std::thread{HandleConnection, connfd};
        
        }
    #endif
}