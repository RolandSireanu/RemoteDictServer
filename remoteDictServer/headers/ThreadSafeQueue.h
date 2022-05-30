#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <deque>
#include <mutex>
#include <thread>

class TSQueue
{
    public:
        TSQueue();
        bool empty();
        int pop();        
        void push(int arg);
        unsigned int size();

    private:
        std::deque<int> dq;
        std::mutex mtx;
};

#endif

