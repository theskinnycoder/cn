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
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(SERVER_TCP_PORT);

    // + Connect to the server
    if (connect(socket_descriptor, (SA)&serv_addr, sizeof(serv_addr)) != 0)
    {
        printf("Connection with the Server Failed...\n");
        exit(0);
    }
    else
        printf("Connected to the Server...\n");

    // + Send message
    char *msg = "Hello from Client";
    send(socket_descriptor, msg, strlen(msg), 0);
    printf("Message sent...");

    // + Recieve Output
    char *recieved_msg;
    read(socket_descriptor, recieved_msg, 1024);
    printf("Message recieved...");
    printf("%s\n", recieved_msg);
    return 0;
}
