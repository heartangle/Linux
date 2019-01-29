#include "udp_client.hpp"

void Tell(std::string proc)
{
    std::cout<<"User:./"<<proc<<"server_ip server_port"<<std::endl;
}
//./udp_client ip port
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Tell(argv[0]);
        exit(1);
    }
    UdpClient us(argv[1], atoi(argv[2]));
    us.InitClient();
    us.Start();
    return 0;
}
