#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    //1.创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    //2.连接服务器端
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.153.128", &serveraddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //给服务器发送数据
    char recvBuf[1024] = {0};
    int i = 0;

    while(1){
        sprintf(recvBuf, "data : %d\n", i++);

        sleep(1);
        write(fd, recvBuf, strlen(recvBuf) + 1);

        //获取服务器发送来的的数据
        int size = read(fd, recvBuf, sizeof(recvBuf));
        if(size == -1){
            perror("read");
            exit(-1);
        } else if(size > 0){
            printf("recv server %s\n", recvBuf);
        } else if(size == 0){
            //表示客户端断开连接
            printf("server is closed...\n");
            break;
        }
    }


    //关闭连接
    close(fd);

    return 0;
}