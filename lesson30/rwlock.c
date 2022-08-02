#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 

//创建1个共享数据
int num = 1;

//创建读写锁
pthread_rwlock_t rwlock;

void * writeNum(void * arg){
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}

void * readNum(void * arg){
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("===read, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}

int main(){
    //初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);
    
    //创建3个写线程，5个读线程
    pthread_t wtids[3], rtids[5];

    //写线程
    for(int i = 0; i < 3; ++i){
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    //读线程
    for(int i = 0; i < 5; ++i){
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    //设置写线程分离
    for(int i = 0; i < 3; ++i){
        pthread_detach(wtids[i]);
    }

    //设置读线程分离
    for(int i = 0; i < 5; ++i){
        pthread_detach(rtids[i]);
    }

    pthread_exit(NULL);
    
    //释放读写锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}