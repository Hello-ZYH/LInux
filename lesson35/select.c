#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>


int main(){
    //创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;


    //绑定
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    

    //监听
    listen(lfd, 8);


    //创建一个fd_set的集合，存放需要检测的文件描述符
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while(1){
        tmp = rdset;

        //调用select系统函数，让内核帮忙检测哪些文件描述符有数据
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        } else if(ret == 0){
            continue;
        } else if(ret > 0){
            //说明检测到了文件描述符的对应的缓冲区的数据发生了改变
            if(FD_ISSET(lfd, &tmp)){
                //表示有新的客户端连接进来了
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);

                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

                //将新的文件描述符加入到集合中
                FD_SET(cfd, &rdset);

                //更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for(int i = lfd + 1; i <= maxfd; ++i){
                if(FD_ISSET(i, &tmp)){
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i, buf, sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    } else if(len == 0){
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &rdset);
                    } else if(len > 0){
                        printf("recv buf = %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
        }

    }

    close(lfd);

    return 0;
}