#ifndef __INFRA_SRC_OS_THREAD_H__
#define __INFRA_SRC_OS_THREAD_H__

#include <string>
#include <functional>

#include "utils/singleton.h"
#include "utils/uncopyable.h"  

namespace Infra {

struct ThreadInfo;

// 虚函数+继承，使用者需要重写基类的纯虚函数，实现自己的逻辑
class CThread
{
    INFRA_DISABLE_COPY(CThread)

public:
    typedef pthread_t           NativeType;
    typedef NativeType*         NativeHandleType;

    enum ThreadPriority
    {
        PRIORITY_MIN = 1,
        PRIORITY_DEFAULT = 50,
        PRIORITY_MAX = 99,
    };

    enum ThreadPolicy
    {
        POLICY_OTHER,   // time-sharing，不支持优先级设置，可以设置nice值
        POLICY_FIFO,    // first in-first out
        POLICY_RR,      // round-robin
    };
    
public:
    CThread(const std::string& name = std::string("defaults"), int policy = POLICY_OTHER, int priority = PRIORITY_DEFAULT);
    virtual ~CThread();
    /// 派生类必须要实现此函数
    virtual void run() = 0;
    bool create();
    bool join();
    bool detach();
    bool cancel();

    std::string getName() const noexcept;
    bool looping() const noexcept;
    bool joinable() const noexcept;
    bool isThreadOver() const noexcept;
    unsigned long int getThreadId() const noexcept;

    static int getCurrentThreadId();
    static void sleepMicroSecond(int ms);     

    NativeHandleType nativeHandle();
    
private:

private:
    ThreadInfo *m_thread_info;
};

// ==========================================================================================================
struct ThreadManagerInfo;

class CThreadManager
{
    INFRA_SINGLETON_DECLARE(CThreadManager)
// public:
//   CThreadManager();
//   ~CThreadManager();

public:
  bool add(ThreadInfo* thread);
  bool remove(ThreadInfo* thread);
  void dump();

private:
    ThreadManagerInfo *m_manager;
};

// ==========================================================================================================
struct ThreadLiteInfo;

// 依赖类的组合，使用function和bind实现委托机制，依赖于回调函数的传入逻辑
class CThreadLite : public CThread
{
public:
    typedef std::function<void ()> ThreadFunc;

public:
    explicit CThreadLite(const ThreadFunc& func, 
                        const std::string& name = std::string("dftlName"), 
                        int policy = POLICY_OTHER, 
                        int priority = PRIORITY_DEFAULT);
    ~CThreadLite();

    void run();

private:
    ThreadLiteInfo *m_threadLiteInfo;

};

}





#endif
