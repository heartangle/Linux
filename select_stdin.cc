#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
int main()
{
    //将0(标准输入)设置进read_fds
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);

    while(1){
        printf("> ");
        fflush(stdout);

        //最大文件描述符为0，只关心读事件，阻塞等待
        int ret = select(1, &read_fds, NULL, NULL, NULL);
        if(ret < 0){
            perror("select error\n");
            continue;
        }
        if(FD_ISSET(0, &read_fds)){//若标准输入就绪
            char buf[1024] = {0};
            read(0, buf, sizeof(buf)-1);
            printf("input:%s\n", buf);
        }
        else{
            continue;
        }
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
    }
    return 0;
}
