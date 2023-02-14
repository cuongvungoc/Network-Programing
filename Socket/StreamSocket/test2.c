#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include<string.h>
int main(void)
{
    DIR *d;
    struct dirent *dir;
    char path[1000]="/home/cuongvn/Cuong/Network-Programing/Socket/StreamSocket";
    d = opendir(path);
    char full_path[1000];
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //Condition to check regular file.
            if(dir->d_type==DT_REG){
                full_path[0]='\0';
                strcat(full_path,path);
                strcat(full_path,"/");
                strcat(full_path,dir->d_name);
                printf("%s\n",full_path);
            }
        }
        closedir(d);
    }
    return(0);     
}