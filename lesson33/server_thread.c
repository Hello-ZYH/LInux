#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

struct sockInfo{
    int fd;   //通信的文件描述符
    struct sockaddr_in addr;
    pthread_t tid;   //线程号
};

struct sockInfo sockinfos[128];

void * working(void * arg){
    //子线程和客户端通信
    //获取客户端的信息
    struct sockInfo * info = (struct sockInfo*)arg;

    char clientIP[16];
    inet_ntop(AF_INET, &info->addr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(info->addr.sin_port);

    printf("client ip is : %s, port is %d\n", clientIP, clientPort);

    //接收客户端发来的数据
    char recvBuf[1024];
    while(1){
        int len = read(info->fd, &recvBuf, sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(-1);
        } else if(len > 0){
            printf("recv client %s\n", recvBuf);
        } else{
            printf("client closed...\n");
            break;
        }
        sleep(1);

        write(info->fd, recvBuf, strlen(recvBuf) + 1);
    }

    close(info->fd);
    
    return NULL;
}


int main(){
    //创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9999);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    //绑定
    int ret = bind(lfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //监听
    ret = listen(lfd, 128);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    //初始化
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for(int i = 0; i < max; ++i){
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    //循环等待客户端进行连接，一旦一个客户端连接进来，就创建一个子线程进行通信
    while(1){
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);

        //接收连接
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
        if(cfd == -1){
            if(errno == EINTR){
                continue;
            }
            perror("accept");
            exit(-1);
        }        

        struct sockInfo * info;
        for(int i = 0; i < max; ++i){
            //从这个数组中找到一个可以用的sockInfo元素
            if(sockinfos[i].fd == -1){ 
                info = &sockinfos[i];
                break;
            }
            if(i = max - 1){
                sleep(1);
                i--;
            }
        }

        info->fd = cfd;
        memcpy(&info->addr, &clientaddr, len);

        //创建子线程
        pthread_create(&info->tid, NULL, working, info);

        pthread_detach(info->tid); 
    }

    close(lfd);

    return 0;
} 
