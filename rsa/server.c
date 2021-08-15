#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define SERV_TCP_PORT 8080
#define SA struct sockaddrint

int main()
{
    int newsockfd, clength;
    char a[100], s[100], l[100];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created...\n");

    struct sockaddr_in serv_addr, cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if ((bind(sockfd, (SA *)&serv_addr, sizeof(serv_addr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    clength = sizeof(cli_addr);
    newsockfd = accept(sockfd, (SA *)&cli_addr, &clength);
    if (newsockfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // -- RSA ALGORITHM
    double e, phi, n, encrypted_data;

    // + Get the public key, Euler-Totient Function, n & encrypted data from the client
    read(newsockfd, e, phi, n, encrypted_data);
    printf("Encrypted Data received from the client...\n");

    // + Assume private encryption key (d)
    double d = (2 * phi + 1) / e;

    // + Decrypt the data
    printf("Decrypting the data....\n");
    double decrypted_data = pow(encrypted_data, d) % n;
    printf("Decrypted data: %lf", decrypted_data);

    // + Close the socket
    close(sockfd);
    return 0;
}
