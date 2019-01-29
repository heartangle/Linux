#include"tcp_client.hpp"

void Tell(std::string proc)
{
    std::cout<<"User:"<<proc<<"ip port"<<std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Tell(argv[0]);
        exit(1);
    }
    TcpClient* ts = new TcpClient(argv[1], atoi(argv[2]));
    ts->InitClient();
    ts->Start();
    return 0;

}
