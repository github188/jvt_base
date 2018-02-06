#include <pthread.h>

class Thread
{
    private:
        pthread_t pid;
    private:
        static void * start_thread(void *arg);                                                       //静态成员函数
    public:
        int start();
        virtual void run() = 0;//基类中的虚函数
};

int Thread::start()
{
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
        if(pthread_create(&pid,NULL,start_thread,(void *)this) != 0) 
        {      
            return -1;
        }      
	pthread_attr_destroy(&attributes);
        return 0;
}

void* Thread::start_thread(void *arg) //静态成员函数只能访问静态变量或静态函数，通过传递this
{
    Thread *ptr = (Thread *)arg;
    ptr->run(); 
	return 0;    
}

