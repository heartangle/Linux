#include"tcp_server.hpp"

void Tell(std::string proc)
{
    std::cout<<"User:./"<<proc<<"ip port"<<std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Tell(argv[0]);
        exit(1);
    }
    TcpServer* ts = new TcpServer(argv[1], atoi(argv[2]));
    ts->InitServer();
    ts->Start();
    return 0;
}
