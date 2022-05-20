#include <gtest/gtest.h>
#include <ThreadSafeDeque.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <memory>

TSDeque<int> globalDeque{};
std::vector<int> whatGoesIn{};
std::mutex globalMutex;


std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(2,50);

void push()
{
    int c = 0;
    while(c<10)
    {
        unsigned int randomValue = dist(rng);
        {
            std::lock_guard<std::mutex> lockGuard(globalMutex);
            globalDeque.push(randomValue);
            whatGoesIn.push_back(randomValue);
        }
        c++;
    }
}

TEST(MultithreadedTests, TestingSizeFunction) {
    std::thread threads[100];
    for(int i=0; i<100; i++)
        threads[i] = std::thread(push);
    
    for(int i=0; i<100; i++)
        threads[i].join();


    for(const auto& e : whatGoesIn)
        ASSERT_EQ(e, *(globalDeque.pop()));

}

