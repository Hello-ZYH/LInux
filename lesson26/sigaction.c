#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void myalarm(int num){
    printf("捕捉到的信号编号是：%d\n", num);
    printf("xxxxxx\n");
}

int main(){
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);  //清空临时阻塞信号集

    //注册信号捕捉
    sigaction(SIGALRM, &act, NULL);

    struct itimerval new_value;

    //设置间隔的时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟的时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    printf("定时器开始了...\n");

    if(ret == -1){
        perror("setitimer");
        exit(0);
    }

    //getchar();
    while(1);
    
    return 0; 
} 
