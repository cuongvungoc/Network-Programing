#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>

#define TIMEOUT 5 /*timeout 5 seconds*/

int main(void)
{
    struct pollfd fds[2];
    int ret = -1;

    /* Kiểm tra stdin sẵn sàng đọc */
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* kiểm tra stdout sẵn sàng ghi */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    ret = poll(fds, 2, TIMEOUT * 1000);

    if (-1 == ret)
    {
        perror("poll() error!");
        return -1;
    }
    if (0 == ret)
    {
        printf("poll() timeout after %d seconds.\n", TIMEOUT);
        return 0;
    }

    /*Kiem tra revents cho stdin va stdout*/
    if (fds[0].revents & POLLIN)
    {
        printf("stdin ready to read\n");
    }
    if (fds[1].revents & POLLOUT)
    {
        printf("stdout ready to write\n");
    }

    return 0;
}
