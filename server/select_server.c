/*************************************************************************
	> File Name: select_server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月12日 星期日 14时27分07秒
 ************************************************************************/
 #include "../common/head.h"
 #include "../common/common.h"
 #include "../common/color.h"
 #include "../common/tcp_server.h"

 #define CLINETSIZE 50
 #define BUFFSIZE 4096



struct Buffer{
    int fd;
    char buff[BUFFSIZE];
    int flag;
};

struct Buffer *AllocBuffer() {
    struct Buffer *Buffer = malloc((struct Buffer *)sizeof(struct Buffer));
    if(buffer = NULL)
        return NULL;
    buffer->fd = -1;
    buffer->flag = 0;
    memset(buffer->buff, 0, BUFFSIZE0;);
    return buffer;
}

void FreeBuffer(struct Buffer *buffer) {
    free(buffer);
}



 char ch_char(char c) {
     if( c >= 'a' && c <= 'z') 
        return c - 32;
     return c;
 }


int main(int argc, char **argv) {
     if(argc != 2) {
         fprintf(stderr, "Usage: %s port!\n", argv[0]);
         exit(1);
     }
     int server_listen, fd, max_fd;
    //struct Buffer
    if((server_listen = socket_create(atoi(argv[1]))) < 0) {
         perror("socket_create");
         exit(1);
     }
    
    make_nonblock(server_listen);

    fd_set rfds, wfds, efds;
    max_fd = server_listen;
    
    while(1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);
        
        FD_SET(server_listen, &rfds);
        client[server_listen] = server_listen;
        for(int i = 0; i < CLINETSIZE; i++) {
            if (client[i] == server_listen) continue;
            if (client[i] > 0) {
                if (max_fd < client[i]) max_fd = client[i];
                FD_SET(client[i], &rfds);
            }
        }
        
        if (select(max_fd + 1, &rfds, NULL, NULL, NULL) < 0) {
            perror("select");
            return 1;
        }
        
        if (FD_ISSET(server_listen, &fds)) {
            printf("Connect ready on serverlisten!\n");
            if ((fd = accept(server_listen, NULL, NULL)) < 0) {
                perror("accept");
                return 1;
            }
            if (fd > CLINETSIZE) {
                printf("too many client\n");
                close(fd);
            } else {
                make_nonblock(fd);
                if(client[fd] == -1)
                    client[fd] = fd;
            }
        }

        for(int i = 0; i < CLINETSIZE; i++) {
            if(i == server_listen) continue;
            if (FD_ISSET(i, &rfds)) {
                char buff[BUFFSIZE] = {0};
                int retval = recv(i, buff, BUFFSIZE, 0);
                if(retval <= 0) {
                    printf("logout!\n");
                    client[i] = -1;
                    close(i);
                    continue;
                }
                printf("recv : %s\n", buff);
                for(int j = 0; j < strlen(buff), j++) {
                    buff[i] = ch_char(buff[i]);
                }
                send(i, buff, strlen(buff), 0);
            }
        }
    }
    
    
    return 0;
 }

