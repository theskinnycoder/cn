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

char *create_string_of_n0s(char *str, int len)
{
    for (register short i = 0; i < len; ++i)
        str[i] = '0';
    return str;
}

char * xor (char *a, char *b)
{
    char *result;
    int n = strlen(b);
    for (register short i = 1; i < n; ++i)
        result += (a[i] == b[i]) ? "0" : "1";
    return result;
};

char *mod2div(char *divident, char *divisor)
{
    int pick = strlen(divisor);
    int len_of_divident = strlen(divident);

    // Slicing the divident to appropriate length for particular step
    char *tmp;
    strncpy(tmp, divident, pick);

    while (pick < len_of_divident)
    {
        if (tmp[0] == '1')
            // Replace the divident by the result of XOR and pull 1 bit down
            tmp = xor(divisor, tmp) + divident[pick];
        else
        {
            // If the leftmost bit of tmp is 0, the step can't use the regular divisor. We need to use an all-0s divisor.
            char *new_divisor;
            tmp = xor(create_string_of_n0s(new_divisor, pick), tmp) + divident[pick];
        }
        // Increment pick to move further
        pick += 1;
    }

    // For the last n bits, we have to carry it outnormally as increased value of pick will cause Index Out of Bounds.
    if (tmp[0] == '1')
        tmp = xor(divisor, tmp);
    else
    {
        char *new_divisor;
        tmp = xor(create_string_of_n0s(new_divisor, pick), tmp);
    }
    return tmp;
}

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

    // + Accepts and returns a new SFD for the 1st connection in the queue
    socket_descriptor = accept(socket_descriptor, (SA)&server_address, &sizeof(server_address));
    if (socket_descriptor < 0)
    {
        printf("Server didn\'t acccept...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client...\n");

    // -- CRC Algorithm
    char *data, *divisor, *remainder;
    read(socket_descriptor, data, sizeof(data));
    read(socket_descriptor, divisor, sizeof(divisor));

    // + Perform ModDiv & send remainder to client
    strcpy(remainder, mod2div(data, divisor));
    write(socket_descriptor, remainder, sizeof(remainder));

    // + Close Socket Connection
    close(socket_descriptor);
    return 0;
}
