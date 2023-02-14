#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
              printf("%s\n", dir->d_name);
            // if (strcmp(dir->d_name, "test") == 0)
            // {
            //       printf("%s\n", dir->d_name);
            // }
        }
        closedir(d);
    }
    return (0);
}