#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(int argc, char* argv[])
{
    //建立监听套接字
    if(argc != 3){
        printf("./server [ip] [port]\n");
        return 1;
    }
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0){
        perror("socker error\n");
        return 2;
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(argv[1]);
    local.sin_port = htons(atoi(argv[2]));
    if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
        perror("bind error\n");
        return 3;
    }
    if(listen(listen_sock,  5) < 0){
        perror("listen error\n");
        return 4;
    }

    //数组的大小就是fd_set这个位图能够存多少个fd,因为需要用数组记录
    int fdArray[sizeof(fd_set)*8];
    
    //把listen_sock给数组的第一个元素
    fdArray[0] = listen_sock;
    
    int num = sizeof(fdArray)/sizeof(fdArray[0]);//最多存在1024个fd

    //然后初始化数组为-1
    for(int i=0; i<num; i++){
        fdArray[i] = -1;
    }

    while(1){
        fd_set rfds;//建立关心读位图
        FD_ZERO(&rfds);//初始化位图
        int maxfd = fdArray[0];//当前的最大fd为listen_sock

        for(int i=0; i<num; i++){
            if(fdArray[i] >= 0){
                FD_SET(fdArray[i], &rfds);
                //更新最大的fd
                if(maxfd < fdArray[i])
                    maxfd = fdArray[i];
            }
        }
        switch(select(maxfd+1, &rfds, NULL, NULL, NULL)){
        case 0:
            printf("超时!\n");
            break;
        case -1:
            printf("出错!\n");
            break;
        default:
            {
                for(int i=0; i<num; i++){
                    if(fdArray[i] == -1)
                        continue;
                    //若fd已经就绪
                    if(FD_ISSET(fdArray[i], &rfds)){
                        //listen_sock就绪
                        if(FD_ISSET(fdArray[i], &rfds) && i==0){
                            //建立连接
                            struct sockaddr_in client;
                            socklen_t len = sizeof(client);
                            int new_sock = accept(listen_sock, (struct sockaddr*)&client, &len);
                            if(new_sock < 0){
                                perror("accept error!\n");
                                return 5;
                            }
                            //遇见-1就说明处理完了
                            for(int i=0; i<num; i++){
                                if(fdArray[i] == -1)
                                    break;
                            }
                            //new_sock必须在范围之内
                            if(i < num)
                                fdArray[i] = new_sock;
                            else
                                continue;
                            }
                        }
                        //普通fd就绪
                        if(FD_ISSET(fdArray[i], &rfds)){
                            char buf[1024];
                            ssize_t s = read(fdArray[i], buf, sizeof(buf)-1);
                            if(s > 0){
                                buf[s] = 0;
                                printf("client#%s\n", buf);
                            }
                            else if(s == 0){
                                printf("client quit!\n");
                                close(fdArray[i]);
                                fdArray[i] = -1;
                            }
                            else{
                                break;
                            }                            
                        }
                }
           }
        }
    }
    return 0;
}
