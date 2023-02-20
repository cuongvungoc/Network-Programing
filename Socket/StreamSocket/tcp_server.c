// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>

#define PORT 8080
#define SIZE 1024
#define MAX_THREAD 5
#define LIST_FILES '1'
#define DOWNLOAD '2'
#define QUIT 'q'

void send_file(FILE *fp, int sockfd)
{
    printf("Send file called!\n");
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("Error in sending file");
            exit(1);
        }
        bzero(data, SIZE);
    }
    fseek(fp, 0L, SEEK_END); // seek to the EOF
    int size = ftell(fp);
    rewind(fp);

    printf("Number of bytes sent: %d bytes\n", size);
    printf("File data sent successfully.\n");
}

void list_file(int new_socket)
{
    struct dirent *dir;
    DIR *d;
    d = opendir(".");
    char quit[100] = "q";
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char buffer[SIZE] = {0};
            send(new_socket, dir->d_name, strlen(dir->d_name), 0);
            read(new_socket, buffer, SIZE);
        }
    }
    send(new_socket, quit, strlen(quit), 0);

    free(dir);
    free(d);
}

void *handle_client(void *param)
{
    char buffer[SIZE] = {0};
    int new_socket = (int *)param;
    read(new_socket, buffer, SIZE);
    if (buffer[0] == QUIT)
    {
        printf("Disconnected from client\n");
        pthread_exit(NULL);
    }
    else if (buffer[0] == LIST_FILES)
    {
        printf("Reading from client!\n");
        list_file(new_socket);
    }
    else if (buffer[0] == DOWNLOAD)
    {
        char file_name[SIZE];
        read(new_socket, file_name, SIZE);
        FILE *fp;
        fp = fopen(file_name, "r");
        if (fp == NULL)
        {
            perror("Error in reading file.\n");
            exit(EXIT_FAILURE);
        }
        send_file(fp, new_socket);
        pthread_exit(NULL);
        // break;
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    int e;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

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
        perror("Error in listening\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[MAX_THREAD];
    int n_thread = 0;
    // pthread_t thread_id;
    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted from: %s, PORT: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        pthread_create(&threads[n_thread++], NULL, handle_client, (int *)new_socket);
    }

    pthread_exit(NULL);

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
