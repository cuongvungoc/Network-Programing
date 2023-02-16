#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(void)
{
    char filename[] = "send.txt";

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Cannot open source file.\n");
        exit(1);
    }

    fseek(fp, 0L, SEEK_END); // seek to the EOF
    int size = ftell(fp);    // get the current position
    rewind(fp);              // rewind to the beginning of file

    printf("%d", size);
    return (0);
}