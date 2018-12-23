
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "os/condition.h"
#include "utils/colorfulprint.h"

using namespace std;
using namespace Infra;

class CConditionTest : public ::testing::Test, public Infra::CCondition
{
public:
  // 以下实现为空的函数体其实可以不写
  CConditionTest()
  {
  }
  ~CConditionTest()
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
TEST_F(CConditionTest, CVSignal)
{
    CCondition cond_var;
    EXPECT_EQ(cond_var.signal(), true);
}

TEST_F(CConditionTest, CVBroadcast)
{
    CCondition cond_var;
    EXPECT_EQ(cond_var.broadcast(), true);    
}

// 不加锁互斥量直接解锁互斥量也应该返回错误
TEST_F(CConditionTest, CVWait)
{
    CMutex mutex;
    CCondition cond_var;
    // 该函数不能单独测试，因为此测试里无法signal或broadcast
    // EXPECT_EQ(cond_var.wait(mutex), false);
}

TEST_F(CConditionTest, CVTimedWait)
{
    CMutex mutex;
    CCondition cond_var;
    EXPECT_EQ(cond_var.timedwait(mutex, 2), false);

}


