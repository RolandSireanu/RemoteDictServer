#include <ThreadSafeQueue.h>


TSQueue::TSQueue() {}


bool TSQueue::empty()
{
    return dq.empty(); 
}


int TSQueue::pop()
{
    std::lock_guard<std::mutex> lockGuard(mtx);
    int tempResult = -1;
    if(dq.size() > 0)             
    {
        tempResult = dq.front();        
        dq.pop_front();
    }
    return tempResult;
}

void TSQueue::push(int arg)
{
    std::lock_guard<std::mutex> lockGuard(mtx);
    dq.push_back(arg);      
}

unsigned int TSQueue::size() 
{ 
    return dq.size();
}