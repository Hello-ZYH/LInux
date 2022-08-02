#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int srcfd = open("english.txt", O_RDONLY);
    if(srcfd == -1){
        perror("open");
        return -1;
    }

    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
    if(destfd == -1){
        perror("open");
        return -1;
    } 

    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0){
        write(destfd, buf, len);
    }

    close(srcfd);
    close(destfd);
    
    return 0;
}