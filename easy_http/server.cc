#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void Usage()
{
    printf("Usage: ./server [ip] [port]\n");
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage();
        return 1;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket error");
        return 2;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind error");
        return 3;
    }
    ret = listen(fd, 10);
    if(ret < 0)
    {
        if(ret < 0)
        {
            perror("listen error");
            return 4;
        }
    }
    for(;;)
    {
        struct sockaddr_in client_addr;
        socklen_t len;
        int client_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
        if(client_fd < 0)
        {
            perror("accept error");
            return 5;
            continue;
        }
        char input_buf[1024*10] = {0};//用足够大的缓冲区把数据读完
        ssize_t read_size = read(client_fd, input_buf, sizeof(input_buf)-1);
        if(read_size < 0)
        {
            return 6;
        }
        printf("[Request]%s", input_buf);
        char buf[1024] = {0};
        const char* hello = "<h1>hello world！！！<h1>";
        sprintf(buf, "HTTP1.1 302 OK\nContent-LengthLocation:www.baidu.com:%lu\n\n%s", strlen(hello), hello);
        write(client_fd, buf, sizeof(buf));
    }
    return 0;
}
