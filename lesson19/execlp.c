#include <unistd.h>
#include <stdio.h>

int main(){
    pid_t pid = fork();

    if(pid > 0){
        //父进程
        printf("i am parent process, pid : %d\n", getpid());
        sleep(1);
    }
    else if(pid == 0){
        //子进程
        //execl("hello", "hello", NULL);

        execlp("ps", "ps", "aux", NULL);

        printf("i am child process, pid : %d\n", getpid());
    }
    
    for(int i = 0; i < 3; ++i){
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}