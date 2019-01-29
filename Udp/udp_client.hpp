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
class UdpClient{
public:
    UdpClient(std::string _ip, int _port):server_ip(_ip),server_port(_port),sock(-1)
    {}

    void InitClient()
    {
        
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock < 0)
        {
            std::cerr<<"socket error"<<std::endl;
            exit(2);
        }
    }

    void Start()
    {
        for(;;)
        {
            std::string in;
            std::cout<<"Please Enter: ";
            std::cin>>in;
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            peer.sin_family = AF_INET;
            peer.sin_addr.s_addr = inet_addr(server_ip.c_str());
            peer.sin_port = htons(server_port);
            char buf[1024];
            //忽略出错处理
            sendto(sock, in.c_str(), in.size(), 0, (struct sockaddr*)&peer, len);
            ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&peer, &len);
            if(s > 0)
            {
                buf[s] = 0;
                std::cout<<buf<<std::endl;
            }
        }
    }
    ~UdpClient()
    {
        if(sock>0)
        {
            close(sock);
        }
    }
private:
    std::string server_ip;
    int server_port;
    int sock;
};

#endif //__UDP_SERVER__
