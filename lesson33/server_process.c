#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recycleChild(int arg){
    while(1){
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1){
            //所有子进程都回收了
            break;
        } else if(ret == 0){
            //还有子进程活着
            break;
        } else if(ret > 0){
            //被回收了
            printf("子进程 %d 被回收了\n", ret);
        }
    }
}

int main(){
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    //注册信号捕捉
    sigaction(SIGCHLD, &act, NULL);

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

    //不断地循环等待客户端连接
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

        //每一个连接进来，就创建一个子进程跟客户端通信
        pid_t pid = fork();
        if(pid == 0){
            //子进程
            //获取客户端的信息
            char clientIP[16];
            inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
            unsigned short clientPort = ntohs(clientaddr.sin_port);

            printf("client ip is : %s, port is %d\n", clientIP, clientPort);

            //接收客户端发来的数据
            char recvBuf[1024];
            while(1){
                int len = read(cfd, &recvBuf, sizeof(recvBuf));
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

                write(cfd, recvBuf, strlen(recvBuf) + 1);
            }

            close(cfd);

            //退出当前子进程
            exit(0);
        }
    }

    close(lfd);

    return 0;
}