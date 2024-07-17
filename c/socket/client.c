#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // 包含这个头文件以获取 inet_ntop 的声明
#include <netdb.h>

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256] = "Hello, Server!";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(1234);

    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    unsigned char *ip_addr = (unsigned char *)server->h_addr;

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    write(sockfd, buffer, strlen(buffer));

    close(sockfd);
    return 0;
}
