#include<iostream>

#include<unistd.h>

int main()
{
    pid_t id;
    id = fork();
    if(id < 0)
    {
        std::cout<<"fork error"<<std::endl;
    }
    else if(id == 0)
    {
        std::cout<<"i am a child, pid = "<<getpid()<<std::endl;
    }
    else
    {
        std::cout<<"i am a parent, ppid ="<<getppid()<<std::endl;
    }
    return 0;
}
