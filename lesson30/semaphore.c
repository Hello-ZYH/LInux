#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <semaphore.h>

//创建一个互斥量
pthread_mutex_t mutex;

//创建两个信号量
sem_t psem;
sem_t csem;

struct Node{
    int num;
    struct Node *next;
};

//头结点
struct Node * head = NULL;

//生产者函数
void * producer(void * arg){
    //不断地创建新的节点，添加到链表中
    while(1){
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);

        struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add ndoe, num : %d, tid : %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
    }

    return NULL;
}

//消费者函数
void * customer(void * arg){
    while(1){
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node * temp = head;

        head = head->next; 
        printf("del node, num : %d, tid : %ld\n", temp->num, pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
    }
    
    return NULL;
}

int main(){
    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    //初始化信号量
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    //创建5个生产者线程和5个消费者线程
    pthread_t ptids[5], ctids[5];

    for(int i = 0; i < 5; ++i){
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    //线程分离
    for(int i = 0; i < 5; ++i){
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    while(1){
        sleep(10);
    }

    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    //退出主线程
    pthread_exit(NULL);
    
    return 0;
}
