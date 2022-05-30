#include <gtest/gtest.h>
#include <ThreadSafeDeque.h>
#include <iostream>
#include <random>

// Demonstrate some basic assertions.
TEST(BasicTests, TestingSizeFunction) {
    TSDeque<int> tsDeque{};
    for(int i=0; i<50; i++)
        tsDeque.push(i);
    
    for(int i=0; i<50; i++){
        tsDeque.pop();
        ASSERT_EQ(tsDeque.size(), 49-i);
    }
}

TEST(BasicTests, TestingSizeFunctionRvalues) {
    TSDeque<int> tsDeque{};
    tsDeque.push(1);
    tsDeque.push(2);
    tsDeque.push(3);
    ASSERT_EQ(tsDeque.size() , 3);
    ASSERT_EQ(*(tsDeque.pop()) , 1);
    ASSERT_EQ(*(tsDeque.pop()) , 2);
    ASSERT_EQ(*(tsDeque.pop()) , 3);
}


TEST(BasicTests, EmptyQueueTest) {
    int counter = 0;    
    TSDeque<int> tsDeque{};
    std::shared_ptr<int> temp = tsDeque.pop();
    if(temp == nullptr)
        ASSERT_EQ(1,1);    
    else
        ASSERT_EQ(1,0);    

}


class DequeTest : public ::testing::Test
{
    protected:
        TSDeque<double> doubleDeque;
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen;
        std::uniform_real_distribution<> dist1;
        static constexpr std::size_t DEQUE_SIZE = 128;
        
        DequeTest(): gen(rd()), dist1(0.0,1.0) 
        {
            for(int i=0; i<DEQUE_SIZE; i++)
                doubleDeque.push(dist1(gen));
        }
};

TEST_F(DequeTest, DoubleDequeTest)
{
    EXPECT_EQ(doubleDeque.size(), DEQUE_SIZE);
    for(int i=0; i<DEQUE_SIZE; i++)
        static_cast<void>(doubleDeque.pop());
    EXPECT_EQ(doubleDeque.empty(), true);
}

TEST_F(DequeTest, DoubleDequeTestSize)
{
    for(int i=0; i<DEQUE_SIZE; i++)
        doubleDeque.push(dist1(gen));
    ASSERT_EQ(doubleDeque.size(), DEQUE_SIZE * 2);
}


