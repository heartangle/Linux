#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<netinet/in.h>
class TcpClient{
public:
    TcpClient(std::string _ip, int _port):ip(_ip),port(_port),server_sock(-1)
    {}
    void InitClient()
    {
        //创建SOCKET
        server_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(server_sock < 0)
        {
            std::cerr<<"socket error"<<std::endl;
            exit(2);
        }
        //创建连接
        struct sockaddr_in peer;
        bzero(&peer, sizeof(peer));
        socklen_t len = sizeof(peer);
        peer.sin_family = AF_INET;
        peer.sin_addr.s_addr = inet_addr(ip.c_str());
        peer.sin_port = htons(port);
        int ret = connect(server_sock, (struct sockaddr*)&peer, len);
        if(ret < 0)
        {
            std::cerr<<"connect error"<<std::endl;
            exit(3);
        }
    }

    void Start()
    {
        for(;;)
        {
            std::string msg;
            std::cout<<"Please Enter:";
            std::cin>>msg;
            write(server_sock, msg.c_str(), msg.size());
            char buf[1024];
            ssize_t s = read(server_sock, buf, sizeof(buf)-1);
            if(s > 0)
            {
                buf[s] = 0;
                msg = buf;
                std::cout<<msg<<std::endl;
            }
            else if(s == 0)
            {
                continue;
            }
        }
    }

    ~TcpClient()
    {
        if(server_sock > 0)
        {
            close(server_sock);
        }
    }
private:
    std::string ip;
    int port;
    int server_sock;
};
#endif //__TCP_CLIENT__
