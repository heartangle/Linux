#include "udp_server.hpp"

void Tell(std::string proc)
{
    std::cout<<"User:"<<proc<<"ip port"<<std::endl;
}
//./udp_server ip port
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Tell(argv[0]);
        exit(1);
    }
    UdpServer us(argv[1], atoi(argv[2]));
    us.InitServer();
    us.Start();
    return 0;
}
