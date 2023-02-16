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
#define LIST_FILES '1'
#define DOWNLOAD '2'
#define QUIT 'q'

void recv_list(int new_socket)
{
    int valread;
    char received[1024] = "Received!";
    printf("In receive list function\n");
    while (1)
    {
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
    }
}

void wirte_file(FILE *fp, int sockfd)
{
    int n;
    char buffer[SIZE];

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
    FILE *fp;

    while (1)
    {
        printf("-----------------------------\n");
        printf("1. List all file on server\n");
        printf("2. Download a file on server\n");
        printf("q. Quit the program\n");
        printf("-----------------------------\n");
        printf("Enter your selection: ");
        scanf("%s", select);

        if (select[0] == QUIT)
        {
            send(sockfd, select, sizeof(select), 0);

            close(sockfd);
            return 0;
        }
        else if (select[0] == LIST_FILES)
        {
            send(sockfd, select, sizeof(select), 0);

            recv_list(sockfd);
        }
        else if (select[0] == DOWNLOAD)
        {
            char dir[SIZE] = "./recv_file/";
            char file_name[SIZE];
            send(sockfd, select, sizeof(select), 0);
            printf("Enter file name to download: ");
            scanf("%s", file_name);
            send(sockfd, file_name, sizeof(select), 0);
            strcat(dir, file_name);
            fp = fopen(dir, "w");
            wirte_file(fp, sockfd);
            // break;
        }
    }

    // closing the connected socket
    free(fp);
    close(sockfd);
    return 0;
}