#include<iostream>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<queue>
//简单的mini_shell支持重定向、管道和一些简单命令
#define MAX 1024
#define NUM 30
char cmd[MAX];
char* argv[NUM];
std::queue<char*> q;

void do_display(){
    std::cout<<"[xxx@localhost shell]$";
    fgets(cmd, MAX, stdin);
    cmd[strlen(cmd)-1] = '\0';
}

void do_analy(char* pcmd){
    int i = 0;
    char* p = NULL;//使用指针是一定要初始化
    p = strtok(pcmd, " ");
    argv[i] = p;
    i++;
    while(1){
        p = strtok(NULL, " ");
        if(p == NULL){
            argv[i] = NULL;
            break;
        }
        argv[i] = p;
        i++;
    }
}

std::queue<char*>& do_pipe_num(){
    while(!q.empty()){
        q.pop();
    }
    char* ptr = cmd;
    q.push(ptr);
    while(*ptr != '\0'){
        if(*ptr == '|'){
            *ptr = '\0';
            ptr++;
            while(*ptr == ' ')
            {
                ptr++;
            }
            q.push(ptr);
        }
        ptr++;
    }
    return q;
}

void do_diredict(char* t){
    char* ptr = t;
    char* file = NULL;
    int fd;
    while(*ptr != '\0'){
        if(*ptr == '>'){
            *ptr = '\0';
            ptr++;

        while(*ptr == ' '){
            ptr++;
        }

        file = ptr;
        while((*ptr != ' ') && (*ptr != '\0')){
            ptr++;
        }
        }
        ptr++;
    }
    fd = open(file, O_CREAT|O_WRONLY, 0644);
    dup2(fd, 1);
    close(fd);
}

void do_fork(std::queue<char*>& q){

    while(!q.empty()){
    pid_t id = fork();
    if(id < 0){
        std::cout<<"fork error"<<std::endl;
    }

    else if(id == 0){
        char* tmp = q.front();
        do_diredict(tmp);
        do_analy(tmp);
        execvp(argv[0], argv);
    }

    else{
        waitpid(id,NULL, 0);
    }
    q.pop();
    }
}


int main(){
    while(1){
        do_display();
        std::queue<char*>& q_pipe = do_pipe_num();
        //do_diredict();
        //do_analy();
        do_fork(q_pipe);
        sleep(1);
    }
    return 0;
}
