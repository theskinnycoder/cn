#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MAX 80
#define SERV_TCP_PORT 8080
#define SA struct sockaddrint

double gcd(double a, double b)
{
    double hcf;
    for (double i = 1; i <= a || i <= b; ++i)
        if (a % i == 0 && b % i == 0)
            hcf = i;
    return hcf;
}

int main()
{
    int connfd;
    struct sockaddr_in serv_addr;
    char d[100];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    if (connect(sockfd, (SA *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        printf("Connection with the Server failed...\n");
        exit(0);
    }
    else
        printf("Connected to the Server..\n");

    // -- RSA ALGORITHM
    double data;
    printf("Enter the data you want to send : ");
    scanf("%lf", &data);
    printf("Entered Data: %lf\n", data);

    // + Assume 2 large prime numbers (p, q)
    double p = 3, q = 11;

    // + Calculate n and Euler-Totient Function (phi)
    double n = p * q, phi = (p - 1) * (q - 1);

    // + Assume public encryption key (e) : Try 'e' from 2 to phi
    double e;
    for (int i = 2; i < phi; ++i)
    {
        if (gcd(i, phi) == 1)
            break;
        e = i;
    }

    // + Encrypt the data
    printf("Encrypting the data....\n");
    double encrypted_data = pow(data, e) % n;
    printf("Encrypted Data: %lf\n", encrypted_data);

    // + Send the encrypted data to the server
    write(sockfd, e, phi, n, encrypted_data);
    printf("Encrypted Data Sent to Server");

    // + Close the socket
    close(sockfd);
    return 0;
}