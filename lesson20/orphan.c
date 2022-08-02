#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    //创建子进程
    pid_t pid = fork();
    int num = 10;

    //判断是父进程还是子进程
    if(pid > 0){
        //如果大于0，返回的是创建的子进程的进程号，当前是父进程
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());
        

    }
    else{
        sleep(5);          
        //当前是子进程
        printf("i am child process, pid : %d, ppid : %d\n", getpid(), getppid());  
    }

    //for循环
    for(int i = 0; i < 3 ; ++i){
        printf("i : %d, pid : %d\n", i, getpid());
        sleep(1);
    }

    return 0; 
}