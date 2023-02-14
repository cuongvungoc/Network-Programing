// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080
#define SIZE 1024

// 1 - list all file on server
// 2 - download a file on server

void recv_list(int new_socket)
{
    int valread;
    // int count = 0;
    char received[1024] = "Received!";
    printf("In receive list function\n");
    while (1)
    {
        // char msg[100];
        // printf("Read %d\n", count++);
        char buffer[1024] = {0};
        valread = read(new_socket, buffer, 1024);
        if (valread > 0)
        {
            send(new_socket, received, sizeof(received), 0);
        }

        if (buffer[0] == 'q')
        {
            break;
        }
        printf("%s\n", buffer);
        // bzero(buffer, SIZE);
    }
}

void wirte_file(int sockfd)
{
    int n;
    FILE *fp;
    char *filename = "recv.txt";
    char buffer[SIZE];
    fp = fopen(filename, "w");

    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        // fwrite(buffer, SIZE, 1, fp);
        bzero(buffer, SIZE);
    }
    printf("Data written in the file successfully.\n");
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

    char select[SIZE];
    while (1)
    {
        printf("-----------------------------\n");
        printf("1. List all file on server\n");
        printf("2. Download a file on server\n");
        printf("-----------------------------\n");
        printf("Enter your selection: ");
        // scanf("%c", &select);
        fgets(select, SIZE, stdin);
        // send(sockfd, select, sizeof(select), 0);
        if (select[0] == 'q')
        {
            send(sockfd, select, sizeof(select), 0);

            close(sockfd);
            return 0;
        }
        else if (select[0] == '1')
        {
            send(sockfd, select, sizeof(select), 0);

            recv_list(sockfd);
        }
        else if (select[0] == '2')
        {
            send(sockfd, select, sizeof(select), 0);

            wirte_file(sockfd);
            // break;
        }
    }

    // closing the connected socket
    close(sockfd);
    return 0;
}