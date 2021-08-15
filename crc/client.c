#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERV_TCP_PORT 8080
#define SA struct sockaddrint

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

bool check_data(char *msg)
{
    for (register short i = 0; i < strlen(msg); ++i)
        if (msg[i] == '1')
            return false;
    return true;
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

    // -- CRC ALGORITHM
    // + Take inputs
    char *data, *divisor;
    printf("Enter the data to be transferred : ");
    scanf("%s", data);
    printf("Enter divisor : ");
    scanf("%s", divisor);

    // + Calculate lengths
    int len_of_data = strlen(data), len_of_divisor = strlen(divisor);

    // + Make a copy of data
    char *copy_of_data;
    strcpy(copy_of_data, data);

    // + Pad the copy of data with 0s
    char *padding;
    strcat(copy_of_data, create_string_of_n0s(padding, len_of_divisor - 1));

    // + Perform ModDiv & add CRC to original unpadded data
    strcat(data, mod2div(copy_of_data, divisor));

    // + Send data & divisor to the server
    write(socket_descriptor, data, sizeof(data));
    write(socket_descriptor, divisor, sizeof(divisor));

    // + Recieve the remainder from server
    char *remainder;
    read(socket_descriptor, remainder, sizeof(remainder));

    // + Final Output after checking for 1s in the remainder
    printf("\nMessage from Server :\n");
    if (check_data(remainder))
        printf("Data succesfully received\n");
    else
        printf("Data got corrupted during transmission\n");

    // + Close Socket Connection
    close(socket_descriptor);
    return 0;
}