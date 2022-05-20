// #include <sys/socket.h>
// #include <sys/types.h>
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <stdarg.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <sys/time.h>
// #include <sys/ioctl.h>
// #include <netdb.h>
// #include <exception>
// #include <stdexcept>
// #include <iostream>

// constexpr unsigned int SERVER_PORT = 80;
// constexpr unsigned int  MAXLINE = 4096;
// using SA = struct sockaddr;


// // void err_n_die()

// int main(int argc, char** argv)
// {
//     int sockfd, n;
//     int sendBytes;
//     struct sockaddr_in serverAddr;
//     char sendLine[MAXLINE];
//     char recvLine[MAXLINE];

//     if (argc < 2)
//         throw "Please provide the destination IP address in cli ";

//     // protocol = 0 = TCP
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(sockfd < 0)
//         throw std::runtime_error("Socket didn't get created !");

//     memset(&serverAddr, 0, sizeof(serverAddr));
//     serverAddr.sin_family = AF_INET;
//     //The htons() function makes sure that numbers are stored in memory in network byte order, which is with the most significant byte first.
//     serverAddr.sin_port = htons(SERVER_PORT);

//     if(inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0)
//         throw std::runtime_error("inet_pton didn't reach a valid result ");

//     if(connect(sockfd, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
//         throw std::runtime_error("Connection error");


//     sprintf(sendLine, "GET / HTTP/1.1\r\n\r\n");
//     sendBytes = strlen(sendLine);

//     if(write(sockfd, sendLine, sendBytes) != sendBytes)
//         throw std::runtime_error("Communication error ");


//     memset(recvLine, 0, MAXLINE);
//     do
//     {
//         n = read(sockfd, recvLine, MAXLINE-1);
//         std::cout << recvLine << std::endl;
//     } while (n > 0);
    
//     exit(0);

//     return 0;
// }