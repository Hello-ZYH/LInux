#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

//需要卖的票数
int tickets  = 100;

void * sellticket(void * arg){
    while(tickets > 0){
        usleep(6000);
        printf("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
        tickets--;
    }
    return NULL;
}

int main(){
    //创建3个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    //回收子线程的资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    //设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    pthread_exit(NULL);  //退出主线程

    return 0;
}