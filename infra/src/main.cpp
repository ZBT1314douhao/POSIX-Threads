
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>     // sleep头文件
#include <string>

#include "json/json.h"
#include "os/mutex.h"
#include "os/thread.h"

using namespace std;

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
            sleep(2);
        }
    }

};

// 使用普通函数
void tlFunc1()
{
    while (1)
    {
        printf("[main.cpp line:47] t1Func1 without parameter\n");
        sleep(2);
    }
}

void tlFunc2(int data)
{
    while (1)
    {
        printf("[main.cpp line:55] thread tlFunc2 data = %d\n", data);
        sleep(2);
    }
}
void tlFunc3(Infra::CThreadLite& tl)
{
    while (tl.looping())
    {
        printf("[main.cpp line:45] tlFunc2 with parameter tl\n");
        sleep(2);
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
            sleep(2);
        }
    }

    void memFunc1()
    {
        while (1)
        {
            printf("[main.cpp line:71] memFunc1 without parameter\n");
            sleep(2);
        }   
    }

    void memFunc2(const std::string& data)
    {
        while (1)
        {
            printf("[main.cpp line:57] class threadlite running, data = %s\n", data.c_str());
            sleep(3);
        }
    }

private:
    Infra::CThreadLite m_tl;

};

int main(int argc, char** argv)
{
    // 消除未使用的参数警告
    (void)argc;
    (void)argv;

    Json::Value config = Json::nullValue;
    config["JsonCppVersion"] = "1.8.4";
    config["CmakeVersion"] = "3.9.5";
    config["CompilerVersion"] = "g++ (Debian 6.4.0-2) 6.4.0 20170724";
    printf("config %s\n", config.toStyledString().c_str());
    
    // CThreadTest1 thread1;
    // thread1.create();
    // sleep(2);
    // thread1.join();

    // 无参数的普通函数
    Infra::CThreadLite tl(tlFunc1, std::string("normal function"), Infra::POLICY_FIFO);
    tl.create();
    // 无参数的成员函数
    CThreadLiteTest tl2Test;
    Infra::CThreadLite tl2(std::bind(&CThreadLiteTest::memFunc1, &tl2Test), std::string("memFunc1 without parameter"), Infra::POLICY_RR);
    tl2.create();

    // 有参数的普通函数
    int data = 8;
    Infra::CThreadLite tl3(std::bind(tlFunc2, data), std::string("normal function"));
    tl3.create();

    // 有参数的成员函数
    // CThreadLiteTest tl2Test;
    Infra::CThreadLite tl4(std::bind(&CThreadLiteTest::memFunc2, std::ref(tl2Test), std::string("memFunc2 parameter")), std::string("memFunc1 without parameter"));
    tl4.create();

    CThreadLiteTest tlTest; 
    tlTest.start(); 
    sleep(2);
    tlTest.stop();

    while (1)
    {
        printf("[main.cpp line:61] main thread running\n");
        sleep(2);
    }

    return 0;
}
    


