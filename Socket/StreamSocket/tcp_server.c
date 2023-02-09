// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};
    int file_size = 0;
    while (fgets(data, SIZE, fp) != NULL)
    {
        file_size += sizeof(data);
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("Error in sending file");
            exit(1);
        }
        bzero(data, SIZE);
    }
    printf("Number of bytes sent: %d bytes\n", file_size);
}

// void *thread_function(void *vargp)
// {
//     sleep(1);
//     printf("Hello World!");
// }

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    int e;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    FILE *fp;
    char *filename = "send.txt";
    // char *filename = "picture.png";
    // char buffer[1024] = {0};

    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Server socket created successfully. \n");

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    e = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (e < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Binding successfull.\n");

    if (listen(server_fd, 3) == 0)
    {
        printf("Listening...\n");
    }
    else
    {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }
    
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen);
    if (new_socket < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in *pV4Addr = (struct sockaddr_in *)&address;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
    printf("Listening from client with IP: %s, PORT: %d\n", str, ntohs(pV4Addr->sin_port));
    
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        perror("Error in reading file.\n");
        exit(EXIT_FAILURE);
    }

    send_file(fp, new_socket);
    printf("File data sent successfully.\n");
   
    
    // // pthread_t tid;
    // // pthread_create(&tid, NULL, thread_function, (void *)&tid);
    // // pthread_join(tid, NULL);

    // /* Send message to client*/
    // while (1)
    // {
    //     char msg[100];
    //     char buffer[1024] = {0};
    //     valread = read(new_socket, buffer, 1024);
    //     // printf("%s\n", buffer);
    //     // scanf("%s", &msg);
    //     fgets(msg, 100, stdin);
    //     send(new_socket, msg, strlen(msg), 0);
    //     printf("Message sent from server\n");
    // }

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}