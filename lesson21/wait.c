#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    pid_t pid;

    //创建5个子进程
    for(int i = 0; i < 5; ++i){
        pid = fork();
        if(pid == 0){
            break;
        }
    }

    if(pid > 0){
        //父进程
        while(1){
            printf("parent, pid = %d\n", getpid()); 

            //int ret = wait(NULL);

            int st;
            int ret = wait(&st);

            if(ret == -1){
                break; 
            }

            if(WIFEXITED(st)){
                printf("退出的状态码：%d\n", WEXITSTATUS(st));
            }
            if(WIFSIGNALED(st)){
                printf("被哪个信号干掉了：%d\n", WTERMSIG(st));
            }

            printf("child die, pid = %d\n", ret);

            sleep(1);
        }  

    } else if(pid == 0){
        //子进程
        while(1){
            printf("child, pid = %d\n", getpid());   
            sleep(1);  
        }
        //exit(1);
   
    }
    return 0;
}