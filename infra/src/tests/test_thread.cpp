
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "gtest/gtest.h"
#include "os/thread.h"
#include "utils/colorfulprint.h"

using namespace std;
using namespace Infra;

class CThreadTest : public ::testing::Test, public Infra::CThread
{
public:
  // 以下实现为空的函数体其实可以不写
  CThreadTest()
  {
      //printf("[test_thread.cpp] CThreadTest constructor\n");
  }
  ~CThreadTest()
  {
      //printf("[test_thread.cpp] CThreadTest deconstructor\n");
  }

protected:
    void run()
    {
        while (looping())
        {

            printf("[test_thread.cpp] thread looping tid = %ld, name = %s, currentThreadId = %d\n",
                   getThreadId(), getName().c_str(), getCurrentThreadId());
            sleepMicroSecond(2000);
        }
    }
    
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

TEST_F(CThreadTest, ThreadCreate)
{
    CThreadTest thread;
    EXPECT_EQ(thread.create(), true);
    EXPECT_EQ(thread.create(), false) << "failed in second create thread";
    EXPECT_EQ(thread.joinable(), true);
    sleepMicroSecond(2000);
    EXPECT_EQ(thread.join(), true);
    // 连接一个已经被连接的线程将导致未定义的行为
    EXPECT_EQ(thread.join(), false) << "failed in second join thread";
    EXPECT_EQ(thread.isThreadOver(), true);
}

TEST_F(CThreadTest, ThreadJoin)
{
    CThreadTest thread;
    EXPECT_EQ(thread.join(), false);    // 未创建线程就直接连接线程
}

TEST_F(CThreadTest, ThreadDetach)
{
    CThreadTest thread;
    EXPECT_EQ(thread.detach(), false);  // 未创建线程就分离线程
    EXPECT_EQ(thread.create(), true);
    EXPECT_EQ(thread.detach(), true);
    EXPECT_EQ(thread.detach(), false);
}

TEST_F(CThreadTest, ThreadCancel)
{
    CThreadTest thread;
    EXPECT_EQ(thread.cancel(), false); // 未创建线程就取消线程
    EXPECT_EQ(thread.create(), true);
    EXPECT_EQ(thread.cancel(), true);
    //EXPECT_EQ(thread.isThreadOver(), true);
}

TEST_F(CThreadTest, GetCurrentThreadId)
{
    int tid = getCurrentThreadId();
    tracef("tid = %d\n", tid);
    tid = getCurrentThreadId();
    tracef("tid = %d\n", tid);
    tid = getCurrentThreadId();
    tracef("tid = %d\n", tid);
}

// thread test
class CThreadTest1 : public Infra::CThread
{
  public:
    CThreadTest1()
    {
        //create();
    }
    ~CThreadTest1()
    {
        printf("[main.cpp line:24] deconstrucotr\n");
        //join();
    }
    void run()
    {

        while (looping())
        {

            printf("[main.cpp line:29] CThreadTest1 looping, tid = %ld, name = %s, currentThreadId = %d\n",
                   getThreadId(), getName().c_str(), getCurrentThreadId());
            sleepMicroSecond(2000);
        }
    }
    };

    // 使用普通函数
    void tlFunc1()
    {
        while (1)
        {
            printf("[main.cpp line:47] t1Func1 without parameter\n");
            CThread::sleepMicroSecond(2000);
        }
    }

    void tlFunc2(int data)
    {
        while (1)
        {
            printf("[main.cpp line:55] thread tlFunc2 data = %d\n", data);
            CThread::sleepMicroSecond(2000);
        }
    }
    void tlFunc3(Infra::CThreadLite & tl)
    {
        while (tl.looping())
        {
            printf("[main.cpp line:45] tlFunc2 with parameter tl\n");
            CThread::sleepMicroSecond(2000);
        }
    }

    // 用CThreadLite组合成新的线程类
    class CThreadLiteTest
    {
      public:
        CThreadLiteTest()
            : m_tl(std::bind(&CThreadLiteTest::memFunc, this))
        {
        }

        void start()
        {
            m_tl.create();
        }

        void stop()
        {
            m_tl.join();
        }

        void memFunc()
        {
            printf("[main.cpp line:90] memFunc entrance\n");
            while (m_tl.looping())
            {
                printf("[main.cpp line:92] memFunc running\n");
                CThread::sleepMicroSecond(2000);
            }
        }

        void memFunc1()
        {
            while (1)
            {
                printf("[main.cpp line:71] memFunc1 without parameter\n");
                CThread::sleepMicroSecond(2000);
            }
        }

        void memFunc2(const std::string &data)
        {
            while (1)
            {
                printf("[main.cpp line:57] class threadlite running, data = %s\n", data.c_str());
                CThread::sleepMicroSecond(2000);
            }
        }

      private:
        Infra::CThreadLite m_tl;
    };

    // {
    //     // 消除未使用的参数警告
    //     (void)argc;
    //     (void)argv;

    //     // CThreadTest1 thread1;
    //     // thread1.create();
    //     // sleep(2);
    //     // thread1.join();

    //     // 无参数的普通函数
    //     Infra::CThreadLite tl(tlFunc1, std::string("normal function"), Infra::POLICY_FIFO);
    //     tl.create();
    //     // 无参数的成员函数
    //     CThreadLiteTest tl2Test;
    //     Infra::CThreadLite tl2(std::bind(&CThreadLiteTest::memFunc1, &tl2Test), std::string("memFunc1 without parameter"), Infra::POLICY_RR);
    //     tl2.create();

    //     // 有参数的普通函数
    //     int data = 8;
    //     Infra::CThreadLite tl3(std::bind(tlFunc2, data), std::string("normal function"));
    //     tl3.create();

    //     // 有参数的成员函数
    //     // CThreadLiteTest tl2Test;
    //     Infra::CThreadLite tl4(std::bind(&CThreadLiteTest::memFunc2, std::ref(tl2Test), std::string("memFunc2 parameter")), std::string("memFunc1 without parameter"));
    //     tl4.create();

    //     CThreadLiteTest tlTest;
    //     tlTest.start();
    //     CThread::sleepMicroSecond(2000);
    //     tlTest.stop();

    //     while (1)
    //     {
    //         printf("[main.cpp line:61] main thread running\n");
    //         CThread::sleepMicroSecond(2000);
    //     }

    //     return 0;
    // }
