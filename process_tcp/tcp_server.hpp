#ifndef __TCP_SERVER__
#define __TCP_SERVER__

#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<signal.h>
class TcpServer{
public:
    TcpServer(int _port):port(_port),listen_sock(-1)
    {}
    void InitServer()
    {
        //创建socket
        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(listen_sock < 0)
        {
            std::cerr<<"socket error"<<std::endl;
            exit(2);
        }
        //(一般不会立即关闭而经历TIME_WAIT的过程）后面想继续重用该socke
        bool opt = false;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(bool));
        //进行绑定
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        socklen_t len = sizeof(local);
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0") //INADDR_ANY就是inet_addr("0.0.0.0")
        local.sin_port = htons(port);
        int ret = bind(listen_sock, (struct sockaddr*)&local, len);
        if(ret < 0)
        {
            std::cerr<<"bind error"<<std::endl;
            exit(3);
        }
        //进行监听
        ret = listen(listen_sock, 5);
        if(ret < 0)
        {
            std::cerr<<"listen error"<<std::endl;
            exit(4);
        }
        signal(SIGCHLD, SIG_IGN);//让父进程忽略点子进程退出时发给父进程的SIGCHLD信号后，操作系统会自己回收子进程的资源
    }

    void Servce(int &sock, std::string &ip, uint16_t &port)
    {
            for(;;)
            {
                char buf[1024];
                bzero(buf, sizeof(buf));
                ssize_t s = read(sock, buf, sizeof(buf)-1);
                if(s > 0)
                {
                    buf[s] = 0;
                    std::string msg = buf;
                    std::cout<<"["<<ip<<" : "<<port<<"]"<<msg<<std::endl;
                    msg += "#server echo#";
                    write(sock, msg.c_str(), msg.size());
                }else if(s == 0)
                {
                    std::cout<<"client quit"<<std::endl;
                    break;
                }
                else
                {
                    std::cerr<<"read error"<<std::endl;
                    close(sock);
                    exit(5);
                }
            }
            close(sock);
    }

    void Start()
    {
        for(;;)
        {
            //进行accept
            struct sockaddr_in peer;
            bzero(&peer, sizeof(peer));
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock,(struct sockaddr*)&peer, &len);
            if(sock < 0)
            {
                continue;
            }
            std::cout<<"Get a new client: "<<sock<<std::endl;
            std::string ip_c = inet_ntoa(peer.sin_addr);
            uint16_t port_c = ntohs(peer.sin_port);
            pid_t id = fork();
            if(id < 0)
            {
                std::cerr<<"fork error"<<std::endl;
                close(sock);
                exit(6);
            }
            else if(id == 0)
            {
                close(listen_sock);
                Servce(sock, ip_c, port_c);
                exit(0);
            }
            else
            {
                //父进程需要关闭sock
                close(sock);
            }
        }
    }

    ~TcpServer()
    {
        if(listen_sock > 0)
        {
            close(listen_sock);
        }
    }
private:
    int port;
    int listen_sock;
};
#endif //__TCP_SERVER__
