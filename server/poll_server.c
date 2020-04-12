/*************************************************************************
	> File Name: poll_server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月12日 星期日 19时29分31秒
 ************************************************************************/
//eg:用poll函数写一个服务器，用来接受多个客户端发来的消息并给客户端发送对应信息
#include"../common/head.h"
#include"../common/common.h"
#include"../common/tcp_server.h"
#include<poll.h>

#define POLLSIZE 100
#define BUFSIZE 1024
//小写->大写
char ch_char(char c){
    if(c >= 'a' && c <= 'z') return c - 32;
    return c;
}

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        exit(1);
    }
    int server_listen, fd;
    
    if((server_listen = socket_create(atoi(argv[1]))) < 0){//将socket bind listen封装
        perror("socket_create");
        exit(1);
    }
    
    struct pollfd event_set[POLLSIZE];
    for(int i = 0; i < POLLSIZE; i++){
        event_set[i].fd = -1;
    }
    //用0号event_set监听server_listen
    event_set[0].fd = server_listen;
    event_set[0].events = POLLIN;
    
    while(1){
        //一次while循环只会加上一个client
        int retval;
        if((retval = poll(event_set, POLLSIZE, -1)) < 0){
            perror("poll");
            return 1;
        }
        //如果server_listen就绪，需要向结构体数组里加入客户端fd，以便进行监听
        if(event_set[0].revents & POLLIN){
            if((fd = accept(server_listen, NULL, NULL)) < 0){
                perror("accept");
                continue;
            }
            retval--;
            //找到最小的下标存放fd
            int i;
            for(i = 1; i < POLLSIZE; i++){
                if(event_set[i].fd < 0){
                    event_set[i].fd = fd;
                    event_set[i].events = POLLIN;//对于服务端，要对fd进行读操作
                    break;
                }    
            }
            if(i == POLLSIZE){
                printf("Too many client!\n");
                close(fd);
            }

        }
        //当有fd就绪时，要对就绪的fd进行读操作并且给客户端发送对应的大写信息
        for(int i = 1; i < POLLSIZE; i++){
            if(event_set[i].fd < 0) continue;
            if(event_set[i].revents & (POLLIN | POLLHUP | POLLERR)){
                retval--;
                char buff[BUFSIZE] = {0};
                if(recv(event_set[i].fd, buff, BUFSIZE, 0) > 0){
                    printf("Recv: %s\n", buff);
                    for(int i = 0; i < strlen(buff); i++){
                        buff[i] = ch_char(buff[i]);
                    }
                    send(event_set[i].fd, buff, strlen(buff), 0);
                    
                }else{//当recv失败时，说明这个客户端已经下线，清楚对应的event_set
                    close(event_set[i].fd);
                    event_set[i].fd = -1;
                }
            }
            if(retval < 0) break;
        }
    }

    return 0;
}

