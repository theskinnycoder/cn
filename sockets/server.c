#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERVER_TCP_PORT 8080
#define SA struct *sockaddrint

int main()
{
    // + Create socket connection
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_descriptor == -1)
    {
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created...\n");

    // + Prepare server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_TCP_PORT);

    // + Attach socket to the port 8080 forcefully
    if ((bind(socket_descriptor, (SA)&server_address, sizeof(server_address))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // + Listen for requests from client in queue of max re-tries of 5
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // + accepts and returns a new SFD for the 1st connection in the queue
    socket_descriptor = accept(socket_descriptor, (SA)&server_address, &sizeof(server_address));
    if (socket_descriptor < 0)
    {
        printf("Server didn\'t acccept...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client...\n");

    // + Send Message
    char *msg = "Hello from Server";
    send(socket_descriptor, msg, strlen(msg), 0);
    printf("Message sent...");

    // + Recieve Output
    char *recieved_msg;
    read(socket_descriptor, recieved_msg, 1024);
    printf("%s\n", recieved_msg);
    return 0;
}
