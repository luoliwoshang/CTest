#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stddef.h>
#include <netinet/in.h>

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(1234);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        close(sockfd);
        exit(1);
    }
    printf("Socket bound to port 1234.\n");

    if (listen(sockfd, 5) < 0)
    {
        perror("ERROR on listening");
        close(sockfd);
        exit(1);
    }
    printf("Listening on port 1234...\n");

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    printf("Connection accepted.\n");

    read(newsockfd, buffer, 255);
    printf("Received message: %s\n", buffer);

    close(newsockfd);
    close(sockfd);
    return 0;
}
