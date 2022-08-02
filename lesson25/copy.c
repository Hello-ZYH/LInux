#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    //1.对原始文件进行内存映射
    int fd = open("english.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //获取文件的大小
    int len = lseek(fd, 0, SEEK_END);


    //2.创建一个新文件并扩展
    int fd1 = open("copy.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1){
        perror("open");
        exit(0);
    }

    truncate("copy.txt", len);
    write(fd1, " ", 1);


    //3.分别做内存映射
    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void * ptr1 = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    if(ptr1 == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //内存拷贝
    memcpy(ptr1, ptr, len);

    //释放资源
    munmap(ptr1, len);
    munmap(ptr, len);

    close(fd1);
    close(fd);

    return 0;
}