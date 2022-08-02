#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    int fd = open("hello.txt", O_RDWR);

    if(fd == -1){
        perror("open");
        return -1;
    }
    int ret = lseek(fd, 100, SEEK_END);

    if(ret == -1){
        perror("lseek");
        return -1;
    }

    write(fd, " ", 1);

    close(fd);

    return 0;
}