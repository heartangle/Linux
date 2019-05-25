#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("./client [ip] [port]\n");
        return 1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket error\n");
        return 2;
    }
    
    int ret = connect(fd, (struct sockaddr*)&server, sizeof(server));
    if(ret < 0){
        perror("connect error\n");
        return 3;
    }

    while(1){
        printf("> ");
        fflush(stdout);
        char buf[1024] = {0};
        read(0, buf, sizeof(buf)-1);

        ssize_t s = write(fd, buf, strlen(buf));
        if(s < 0){
            perror("write error\n");
            return 4;
        }
    }
    close(fd);
    return 0;
}
