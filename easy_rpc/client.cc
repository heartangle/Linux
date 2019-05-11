#include"protocol.h"

int sock = -1;

int main()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        std::cerr<<"socket error"<<std::endl;
        exit(1);
    }
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(8888);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");//可以替换为远程ip
    if(connect(sock, (struct sockaddr*)&peer, sizeof(peer)) != 0)
    {
        std::cerr<<"client error"<<std::endl;
        exit(2);
    }
    int result1 = -1;
    for(int i=0; i<10; i++)
    {
        result1 = add(10, 20);
        std::cout<<"result1 is:"<<result1<<std::endl;
        sleep(1);
    }
    close(sock);
    return 0;
}
