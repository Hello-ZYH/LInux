#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    char buf[128];
    getcwd(buf, sizeof(buf));

    printf("当前的工作目录是：%s\n", buf);

    int ret = chdir("/home/zyh/Linux/lesson13");

    if(ret == -1){
        perror("chdir");
        return -1;
    }

    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0664);
    if(fd == -1){
        perror("open");
        return -1;
    }   

    close(fd);

    char buf1[128];
    getcwd(buf1, sizeof(buf1));

    printf("当前的工作目录是：%s\n", buf1);

    return 0;
}