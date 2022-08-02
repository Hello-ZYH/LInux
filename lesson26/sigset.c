#include <stdio.h>
#include <signal.h>

int main(){
    //创建一个信号集
    sigset_t set;

    //清空信号集的内容
    sigemptyset(&set);

    //判断 SIGINT 是否在信号集set中
    int ret = sigismember(&set, SIGINT);
    if(ret == 0){
        printf("SIGINT 不阻塞\n");
    } else if(ret == 1){
        printf("SIGINT 阻塞\n");
    }

    //添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //判断 SIGINT 是否在信号集set中
    ret = sigismember(&set, SIGINT);
    if(ret == 0){
        printf("SIGINT 不阻塞\n");
    } else if(ret == 1){
        printf("SIGINT 阻塞\n");
    }

    //判断 SIGQUIT 是否在信号集set中
    ret = sigismember(&set, SIGQUIT);
    if(ret == 0){
        printf("SIGQUIT 不阻塞\n");
    } else if(ret == 1){
        printf("SIGQUIT 阻塞\n");
    }

    //删除 SIGQUIT 信号
    sigdelset(&set, SIGQUIT);

    //判断 SIGINT 是否在信号集set中
    ret = sigismember(&set, SIGQUIT);
    if(ret == 0){
        printf("SIGQUIT 不阻塞\n");
    } else if(ret == 1){
        printf("SIGQUIT 阻塞\n");
    }
    return 0;
}