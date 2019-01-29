#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<queue>


typedef int (*HandlerTask_t) (int x, int y, int op);

int Cal(int x, int y, int op)
{
    int ret;
    switch(op)
    {
    case 0:
        ret = x+y;
        break;
    case 1:
        ret =  x-y;
        break;
    case 2:
        ret =  x*y;
        break;
    case 3:
        ret =  x/y;
        break;
    default:
        std::cout<<"run error!"<<std::endl;
    }
    return ret;
}

class Task{
public:

    Task(int _x=-0, int _y=-1, int _op= 0)
        :x(_x)
         ,y(_y)
         ,op(_op)
    {}

    void Register(HandlerTask_t handler_)
    {
        handler = handler_;
    }

    void Run()
    {
        int ret = handler(x, y, op);
        const char* op_ = "+-*/";
        std::cout<<"thread: "<<x<<op_[op]<<y<<"="<<ret<<std::endl;
    }
    ~Task()
    {}

    

private:
    int x;
    int y;
    int op;
    HandlerTask_t handler;
};

class ThreadPool{
public:
    ThreadPool(int _num = 5)
        :pthread_num(_num)
         ,idle_nums(0)
    {
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
    }

    void Create_Thread()
    {
        pthread_t t;
        for(int i=0; i<pthread_num; i++)
        {
            pthread_create(&t , NULL, ThreadWork, this);
        }

    }

    void LockQueue()
    {
        pthread_mutex_lock(&lock);
    }

    void UnlockQueue()
    {
        pthread_mutex_unlock(&lock);
    }

    bool IsEmpty()
    {
        return tq.size() == 0 ? true : false;
    }

    void ThreadWait()
    {
        idle_nums++;
        pthread_cond_wait(&cond, &lock);
        idle_nums--;
    }

    void NoticeThread()
    {
        pthread_cond_signal(&cond);
    }

    void GetTask(Task &t)
    {
        t = tq.front();
        tq.pop();
    }

    static void* ThreadWork(void* arg)
    {
        pthread_detach(pthread_self());
        ThreadPool* p = (ThreadPool*)arg;

        while(1)
        {
            p->LockQueue();
            while(p->IsEmpty())
            {
                p->ThreadWait();
            }
            Task t;
            p->GetTask(t);
            p->UnlockQueue();
            t.Run();
        }
    }

    void PushTask(const Task& t)
    {
        LockQueue();
        tq.push(t);
        NoticeThread();
        UnlockQueue();
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }
private:
    std::queue<Task> tq;
    int pthread_num;
    int idle_nums;
    pthread_mutex_t lock;
    pthread_cond_t cond;
};
int main()
{
    ThreadPool pp;
    pp.Create_Thread();
    srand((unsigned int)time(NULL));
    while(1)
    {
    //sleep(1);
    int x = rand()%100+1;
    int y = rand()%100+1;
    int op = rand()%4;
    Task t(x, y, op);
    t.Register(Cal);
    pp.PushTask(t);
    sleep(1);
    }


    return 0;
}
