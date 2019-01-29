#ifndef __TCP_SERVER__
#define __TCP_SERVER__

#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<netinet/in.h>
class TcpServer{
public:
    TcpServer(std::string _ip, int _port):ip(_ip),port(_port),listen_sock(-1)
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
        //进行绑定
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        socklen_t len = sizeof(local);
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = inet_addr(ip.c_str());
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
    }

    void Servce(int &sock)
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
                    std::cout<<msg<<std::endl;
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
            Servce(sock);
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
    std::string ip;
    int port;
    int listen_sock;
};
#endif //__TCP_SERVER__
