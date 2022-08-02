#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main(){
    //创建一个管道
    int fd[2];
    int ret = pipe(fd);

    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    
    //创建子进程
    pid_t pid = fork();

    if(pid > 0){
        //父进程

        //关闭写端
        close(fd[1]);

        //从管道中读取
        char buf[1024];

        int len = -1;
        while((len = read(fd[0], buf, sizeof(buf) - 1)) > 0){
            printf("%s", buf);
            memset(buf, 0, 1024);
        }
        wait(NULL);


    } else if(pid == 0){
        //子进程

        //关闭读端
        close(fd[0]);

        //文件描述符重定向 stdout_fileno -> fd[1]
        dup2(fd[1], STDOUT_FILENO);
        
        //执行 ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);

    } else{
        perror("fork");
        exit(0);
    }
    return 0;
}