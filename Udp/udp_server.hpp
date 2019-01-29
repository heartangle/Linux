#ifndef __UDP_SERVER__
#define __UDP_SERVER__

#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
class UdpServer{
public:
    UdpServer(std::string _ip, int _port):ip(_ip),port(_port),sock(-1)
    {}

    void InitServer()
    {
        
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock < 0)
        {
            std::cerr<<"socket error"<<std::endl;
            exit(2);
        }
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = inet_addr(ip.c_str());
        local.sin_port = htons(port);
        int ret =  bind(sock,(struct sockaddr*)&local, sizeof(local));
        if(ret < 0)
        {
            std::cerr<<"bind error"<<std::endl;
            exit(3);
        }
    }

    void Start()
    {
        for(;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            char buf[1024];
            ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&peer, &len);
            //忽略出错处理
            std::string msg = buf;
            msg += "#server echo#";
            sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*)&peer, len);
        }
    }
    ~UdpServer()
    {
        if(sock>0)
        {
            close(sock);
        }
    }
private:
    std::string ip;
    int port;
    int sock;
};

#endif //__UDP_SERVER__
