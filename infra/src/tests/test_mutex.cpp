
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "../os/mutex.h"
#include "../utils/colorfulprint.h"

using namespace std;
using namespace Infra;

class CMutexTest : public ::testing::Test, public Infra::CMutex
{
public:
  // 以下实现为空的函数体其实可以不写
  CMutexTest()
  {
  }
  ~CMutexTest()
  {
  }

protected:
    
    // 如果上面的构造、析构函数不能满足测试要求，可以添加下面两个函数
    void SetUp() override
    {

    }
    void TearDown() override
    {

    }
    void TestBody()
    {

    }

};

// 只lock不unlock在销毁互斥量的时候返回错误
// 因此测试用例中如果加锁，则最后必须解锁，否则会在锁析构时断言失败
TEST_F(CMutexTest, MutexLock)
{
    CMutex mutex;
    EXPECT_EQ(mutex.lock(), true);  
    EXPECT_EQ(mutex.lock(), false);  // 不能连续加锁同一把锁
    EXPECT_EQ(mutex.unlock(), true); 
}

TEST_F(CMutexTest, MutexTryLock)
{
    CMutex mutex;
    EXPECT_EQ(mutex.trylock(), true);  
    EXPECT_EQ(mutex.trylock(), false);  // 第一次加锁成功，第二次再尝试加锁应该返回失败
    EXPECT_EQ(mutex.unlock(), true);    
}

// 不加锁互斥量直接解锁互斥量也应该返回错误
TEST_F(CMutexTest, MutexUnlock)
{
    CMutex mutex;
    EXPECT_EQ(mutex.unlock(), false);
}

TEST_F(CMutexTest, MutexAll)
{
    CMutex mutex;
    EXPECT_EQ(mutex.lock(), true);  // 只lock不unlock在销毁互斥量的时候返回错误
    EXPECT_EQ(mutex.lock(), false);  // 不能连续加锁同一把锁
    EXPECT_EQ(mutex.unlock(), true);

    EXPECT_EQ(mutex.trylock(), true);
    EXPECT_EQ(mutex.trylock(), false);
    EXPECT_EQ(mutex.unlock(), true);

    EXPECT_EQ(mutex.trylock(), true);
    EXPECT_EQ(mutex.lock(), false);
    EXPECT_EQ(mutex.unlock(), true);

    EXPECT_EQ(mutex.lock(), true);
    EXPECT_EQ(mutex.trylock(), false);
    EXPECT_EQ(mutex.unlock(), true);

}

