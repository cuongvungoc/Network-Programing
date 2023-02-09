// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define SIZE 1024

void wirte_file(int sockfd)
{
    int n;
    FILE *fp;
    // char *filename = "recv.txt";
    char *filename = "recv.png";
    char buffer[SIZE];

    fp = fopen(filename, "wb");
    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
            return;
        }
        // fprintf(fp, "%s", buffer);
        fwrite(buffer, SIZE, 1, fp);
        bzero(buffer, SIZE);
    }
    return;
}

int main(int argc, char const *argv[])
{
    char *ip = "127.0.0.1";
    int e;

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    printf("Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connected to Server.\n");

    wirte_file(sockfd);
    printf("Data written in the file successfully.\n");
    
    /* Read message from Server*/
    // while (1)
    // {
    //     char msg[100];
    //     char buffer[1024] = {0};
    //     send(sock, hello, strlen(hello), 0);
    //     // printf("Message sent from client\n");
    //     valread = read(sock, buffer, 1024);
    //     if (buffer[0] == 'q')
    //     {
    //         break;
    //     }
    //     printf("%s", buffer);
    //     fgets(msg, 100, stdin);
    //     send(sock, msg, strlen(msg), 0);
    //     // free(buffer);
    // }

    // closing the connected socket
    close(sockfd);
    return 0;
}
