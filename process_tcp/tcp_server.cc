#include"tcp_server.hpp"

void Tell(std::string proc)
{
    std::cout<<"User:./"<<proc<<" port"<<std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Tell(argv[0]);
        exit(1);
    }
    TcpServer* ts = new TcpServer(atoi(argv[1]));
    ts->InitServer();
    ts->Start();
    return 0;
}
