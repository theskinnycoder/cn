#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERVER_TCP_PORT 8080
#define SA struct *sockaddrint

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
    write(socket_descriptor, e, phi, n, encrypted_data);
    printf("Encrypted Data Sent to Server");

    // + Close the socket
    close(sockfd);
    return 0;
}