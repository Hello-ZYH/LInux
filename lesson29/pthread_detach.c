#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg){
    printf("child thrad id : %ld\n", pthread_self());
    return NULL;
}

int main(){
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0){
        char * errtsr = strerror(ret);
        printf("error : %s\n", errtsr);
    }

    //输出主线程和子线程的ID
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());

    //设置子线程分离
    pthread_detach(tid);

    pthread_exit(NULL);

    return 0;
}