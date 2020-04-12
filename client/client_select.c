/*************************************************************************
	> File Name: client_select.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月12日 星期日 19时22分37秒
 ************************************************************************/
#include "../common/head.h"
#include "../common/tcp_client.h"
//#include "../common/common.h"

int main(int argc, char **argv) {
    char msg[512] = {0};
    int sockfd;
    if (argc != 4) {
        fprintf(stderr, "Usage: %s ip port timeout\n", argv[0]);
        return 1;
    }

    if ((sockfd = socket_connect_timeout(argv[1], atoi(argv[2]), atoi(argv[3]))) < 0) {
        perror("socket_connect");
        return 2;
    }
    printf("connected!\n");
    sleep(10);
    close(sockfd);
    return 0;
} 

