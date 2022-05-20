#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE
#include <deque>
#include <mutex>
#include <thread>
#include <iostream>
#include <memory>

class empty_queue : std::exception 
{
    public:
        const char* what() const noexcept 
        {
            return "Empty queue exception";
        }
};

template<typename T>
class TSDeque
{
    public:
        template<typename ... Args>
        TSDeque(Args&& ... arg );        
  
  		bool empty();
  
  		std::shared_ptr<T> pop();

        // void push(T&& arg);

        void push(const T& arg);

        unsigned int size();

    private:
        std::deque<T> dq;
        std::mutex mtx;
};

template<typename T>
template<typename ... Args>
TSDeque<T>::TSDeque(Args&& ... arg )
{
    dq = std::deque<T>(std::forward<Args>(arg)...);
}

template<typename T>
bool TSDeque<T>::empty()
{
    return dq.empty(); 
}

template<typename T>
std::shared_ptr<T> TSDeque<T>::pop()
{
    std::lock_guard<std::mutex> lockGuard(mtx);
    std::shared_ptr<T> tempResult;
    if(dq.size() > 0)             
    {
        auto& result = dq.front();
        tempResult = std::make_shared<T>(result);
        dq.pop_front();
    }
    return tempResult;
}

template<typename T>
void TSDeque<T>::push(const T& arg)
{
    std::lock_guard<std::mutex> lockGuard(mtx);
    dq.push_back(T(arg));      
}

template<typename T>
unsigned int TSDeque<T>::size() { return dq.size(); }

#endif