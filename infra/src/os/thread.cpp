
#include "thread.h"

#include <sys/syscall.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>     // sleep头文件

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <list>

#include <pthread.h>

#include "mutex.h"
#include "thread_specific_data.h"
#include "utils/colorfulprint.h"

namespace Infra {

struct ThreadInfo
{
    pthread_t pthread_id;
    CThread *thread;
    CThreadManager *manager;
    std::string name;
    int priority;
    int policy;
    bool looping;
    //bool running;
    bool joinable;
    bool destoryed; // 线程是否被销毁（如，被连接、被取消等）
};

/// \brief 在调用pthread_create时使用
/// 成员函数含有一个隐式参数，导致函数的指针类型并不是void *(*start_routine) (void *)
void* threadBody(void *arg)
{
    ThreadInfo *thread_info = static_cast<ThreadInfo *>(arg);
    if (thread_info->policy == CThread::POLICY_OTHER)
    {
        int priority = -19 + (thread_info->priority * 39) / (Infra::CThread::PRIORITY_MAX + 1);
        /// \brief 
        /// \param which PRIO_PROCESS, PRIO_PGRP, or PRIO_USER
        /// \param who 0表示对应的调用进程、调用进程的进程组、调用线程的真是User ID
        /// \param prio 取值范围-20（最高优先级）到19（最低优先级），默认为0
        setpriority(PRIO_PROCESS, 0, priority);
    }
    thread_info->manager->add(thread_info);
    thread_info->thread->run();
    thread_info->manager->remove(thread_info);
    tracef("thread terminated, releas resources\n");
    delete thread_info;

    return NULL;    // 相当于隐式调用pthread_exit
}

CThread::CThread(const std::string& name /*= std::string("defaults")*/, int policy /*= POLICY_OTHER*/, int priority /*= PRIORITY_DEFAULT*/)
{   
    // 可移植性降低
    static_assert(std::is_same<pid_t, int>::value);
    static_assert(std::is_same<pthread_t, unsigned long int>::value);

    m_thread_info = new ThreadInfo;
    m_thread_info->thread = this;
    m_thread_info->manager = CThreadManager::instance();
    m_thread_info->name = name;
    m_thread_info->priority = priority;
    m_thread_info->policy = policy;
    m_thread_info->looping = false;
    //m_thread_info->running = false;
    m_thread_info->joinable = true;
    m_thread_info->destoryed = false;
    tracef("thread construct successs, policy = %d, priority = %d\n", policy, priority);
}

CThread::~CThread()
{
    tracef("CThread deconstructor\n");
    if (m_thread_info->looping && !m_thread_info->destoryed)
    {
        tracef("detach pthread_id = %ld\n", m_thread_info->pthread_id);
        detach();
    }
}

bool CThread::create()
{
    if (m_thread_info->looping)
    {
        tracef("failed in create, pthread_id = %ld\n", m_thread_info->pthread_id);
        return false;
    }

    // 线程属性
    // 分离或连接状态(default: PTHREAD_CREATE_JOINABLE)
    // 调度继承(default: PTHREAD_INHERIT_SCHED)
    // 调度策略(default: SCHED_OTHER), 调度参数（一般struct sched_param结构体只有一个调度优先级成员）,
    // 调度竞争范围(default: PTHREAD_SCOPE_SYSTEM, linux不支持PTHREAD_SCOPE_PROCESS)
    // 栈大小、栈地址, 栈警戒缓冲区大小（guard(overflow) size）
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);    // 默认值，可不设
    // 实时线程,最小优先级1，最大优先级99
    if (m_thread_info->policy == POLICY_FIFO || m_thread_info->policy == POLICY_RR)
    {
        int ret = pthread_attr_setschedpolicy(&attr, m_thread_info->policy);
        if (ret != 0)
        {
            tracef("failed in pthread_attr_setschedpolicy, ret = %d\n", ret);
            return false;
        }
        int minPriority = (m_thread_info->policy == POLICY_FIFO) ? sched_get_priority_min(SCHED_FIFO) : sched_get_priority_min(SCHED_RR);
        int maxPriority = (m_thread_info->policy == POLICY_FIFO) ? sched_get_priority_max(SCHED_FIFO) : sched_get_priority_max(SCHED_RR);
        if (m_thread_info->priority < minPriority || m_thread_info->priority > maxPriority)
        {
            m_thread_info->priority = (minPriority + maxPriority) / 2;
        }
        struct sched_param param;
        param.sched_priority = m_thread_info->priority;
        if ((ret = pthread_attr_setschedparam(&attr, &param)) != 0)
        {
            tracef("failed in pthread_attr_setschedparam, ret = %d\n", ret);
            return false;
        }
        // 需要手动控制调度策略、优先级、竞争范围时，必须设为PTHREAD_EXPLICIT_SCHED
        if ((ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) != 0)
        {
            errorf("failed in pthread_attr_setinheritsched, ret = %d\n", ret);
            return false;
        }
        // 设置竞争范围 Linux支持PTHREAD_SCOPE_SYSTEM,不支持PTHREAD_SCOPE_PROCESS.为了
        // POSIX.1标准指定默认竞争范围由实现定义，此处为PTHREAD_SCOPE_SYSTEM
        // 下面设置此项返回值为 ENOTSUP，即不支持此项
        //pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS); 
        tracef("real time thread: policy = %d, minPriority = %d, maxPriority = %d, priority = %d\n",
        m_thread_info->policy, minPriority, maxPriority, m_thread_info->priority);
    }
    else    // 非实时线程，SCHED_OTHER最小、最大优先级均为0
    {
        // only for debug 
        // int policy = 0;
        // pthread_attr_getschedpolicy(&attr, &policy);
        // struct sched_param param;
        // pthread_attr_getschedparam(&attr, &param);
        // tracef("non real time thread: policy = %d, priority = %d\n", policy, param.sched_priority);
        if (m_thread_info->policy != POLICY_OTHER)
        {
            tracef("policy(%d) invalid, adjust to POLICY_OTHER\n", m_thread_info->policy);
        }
        int ret = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        if (ret != 0)
        {
            errorf("failed in pthread_attr_setschedpolicy, ret = %d\n", ret);
            return false;
        }
    }

    m_thread_info->looping = true;
    // static函数无法访问某一对象的成员，调用pthread_create时，将this指针作为回调函数的参数。
    // 新创建的线程通过调用threadBody开始执行，传入的参数为 m_thread_info
    // 新线程终止的三种方式： 1.调用pthread_exit 2.从threadBody中返回(相当于调用pthread_exit),从主线程中返回除外 
    // 3.被取消（通过调用pthread_cancel）
    // 进程中的任何线程调用exit或者main函数返回，都会导致进程中的所有线程终止
    int ret = pthread_create(&m_thread_info->pthread_id, &attr, threadBody, m_thread_info);
    if (ret != 0)
    {
        // 返回错误时，第一个参数的值（线程ID）时未定义的
        errorf("failed in pthread_create, ret = %d\n", ret);
        return false;
    }
    if ((ret = pthread_attr_destroy(&attr)) != 0)
    {
        errorf("failed in pthread_attr_destroy, ret = %d\n", ret);
        return false;
    }
    tracef("create thread ok, pthread id = %ld, looping = %d\n", 
            m_thread_info->pthread_id, m_thread_info->looping);
    
    return true;
}

/// \brief 直接停止线程体的循环，不能保证线程此时一定运行完毕
/// \note 连接一个已经被连接过的线程将导致未定义的行为
bool CThread::join()
{
    if (!m_thread_info->joinable || !m_thread_info->looping || m_thread_info->destoryed)
    {
        errorf("failed in thread(pthread id = %ld), joinable = %d, destoryed = %d\n", 
                m_thread_info->pthread_id, m_thread_info->joinable, m_thread_info->destoryed);   
        return false;
    }
    m_thread_info->looping = false;
    // 等待 m_thread_info->pthread_id 指定的线程（必须是joinable）终止，
    // 如果该线程已经终止, 则 pthread_join 将立即返回
    // Tips: 两个线程互相连接（A连接B的时候，B也在连接A）或连接调用线程将导致死锁
    int ret = pthread_join(m_thread_info->pthread_id, NULL);
    if (ret != 0)
    {
        errorf("failed in pthread_join, pthread_id = %ld, ret = %d\n",
                m_thread_info->pthread_id, ret);
        return false;
    }
    m_thread_info->destoryed = true;

    return true;
}

// 分离线程，当已分离的线程终止的时候，其资源将由系统自动回收
// note 尝试detach一个已经分离的线程将导致未定义的行为
bool CThread::detach()
{
    if (!m_thread_info->joinable || m_thread_info->destoryed)
    {
        errorf("failed in detach, thread joinable state = %d, destoryed = %d\n", 
                m_thread_info->joinable, m_thread_info->destoryed);
        return false;
    }
    int ret = pthread_detach(m_thread_info->pthread_id);
    if (ret != 0)
    {
        errorf("failed in pthread_detach(%ld)\n", m_thread_info->pthread_id);
        return false;
    }
    m_thread_info->joinable = false;

    return true;
}

// \brief 向指定线程发送取消请求，当被取消的线程终止后，用pthread_join连接该线程
// 将获取到 PTHREAD_CANCELED 作为线程的退出状态。
// \note 知道取消是否完成的唯一方式是连接该线程
bool CThread::cancel()
{
    if (!m_thread_info->looping || m_thread_info->destoryed)
    {
        errorf("failed in cancel thread, looping = %d, destoryed = %d\n",
                m_thread_info->looping, m_thread_info->destoryed);
        return false;
    }
    // 向指定的线程发送取消请求，目标线程是否以及何时响应取消请求依赖于目标线程的两个属性：
    // state: 取消状态，决定是否响应取消请求，enabled(the default for new threads) or disabled
    // type: 取消类型，决定什么时候取消， asynchronous or deferred (the default for new threads)
    // asynchronous 异步取消，线程可以在任何时候被取消（通常是（但不能保证）立即取消）
    // deferred 延迟取消，取消请求将被推迟直到线程调用了一个是取消点的函数
    // 当被取消的线程终止后，pthread_join将使用PTHREAD_CANCELED作为线程退出状态
    int ret = pthread_cancel(m_thread_info->pthread_id);
    if (ret != 0)
    {
        errorf("failed in pthread_cancel ret = %d\n", ret);
    }
    m_thread_info->looping = false;

    return true;
}

std::string CThread::getName() const noexcept
{
    return m_thread_info->name;
}

bool CThread::looping() const noexcept
{
    return m_thread_info->looping;
}

bool CThread::joinable() const noexcept
{
    return m_thread_info->joinable;
}

bool CThread::isThreadOver() const noexcept
{
    tracef("thread_id = %ld, looping = %d, destoryed = %d\n",
            m_thread_info->pthread_id, m_thread_info->looping, m_thread_info->destoryed);
    return (!m_thread_info->looping && m_thread_info->destoryed);
}

unsigned long int CThread::getThreadId() const noexcept
{
    // 相当于调用pthread_self
    return m_thread_info->pthread_id;
}

// 连续调用会损耗性能，考虑用线程私有数据作为缓存保存
int CThread::getCurrentThreadId()
{ 
    static CThreadSpecificData tsd_tid;
    void *cached_tid = nullptr;
    tsd_tid.get(&cached_tid);
    if (cached_tid == nullptr)
    {
        long tid = static_cast<int>(::syscall(SYS_gettid));
        tsd_tid.set((void *)tid);
        return tid;
    }

    return (long)cached_tid;
    // return static_cast<int>(::syscall(SYS_gettid));
}

void CThread::sleepMicroSecond(int ms)
{
    struct timeval tval = { 0, 0 };
    tval.tv_sec = 0;
    tval.tv_usec = 1000 * ms;
    (void)select(0, NULL, NULL, NULL, &tval);
}

CThread::NativeHandleType CThread::nativeHandle()
{
    return &m_thread_info->pthread_id;
}

// ==========================================================================================================
struct ThreadManagerInfo
{
    std::list<ThreadInfo*> thread_info;
    CMutex thread_info_mutex;
};

INFRA_SINGLETON_IMPLEMENT(CThreadManager)

CThreadManager::CThreadManager()
{
    m_manager = new ThreadManagerInfo;
}

CThreadManager::~CThreadManager()
{
    delete m_manager;
}

bool CThreadManager::add(ThreadInfo* thread)
{
    m_manager->thread_info.push_back(thread);
    warnf("add new thread\n");
    return true;
}

bool CThreadManager::remove(ThreadInfo* thread)
{
    auto it = std::find(m_manager->thread_info.begin(), m_manager->thread_info.end(), thread);
    if (it != m_manager->thread_info.end())
    {
        m_manager->thread_info.erase(it);
        warnf("delete thread\n");
        return true;
    }

    errorf("failed in remove thread from thread manager\n");
    return false;
}

void CThreadManager::dump()
{
    for (auto it = m_manager->thread_info.begin(); it != m_manager->thread_info.end(); ++it)
    {
        fatalf("thread name = %s\n", (*it)->name.c_str());
    }
}

// ==========================================================================================================
struct ThreadLiteInfo
{
    CThreadLite::ThreadFunc func;
};

CThreadLite::CThreadLite(const ThreadFunc& func, 
                        const std::string& name /*= std::string("dftlName")*/, 
                        int policy /*= POLICY_OTHER*/, 
                        int priority /*= PRIORITY_DEFAULT*/)
    : CThread(name, policy, priority)
{
    tracef("threadlite name = %s\n", name.c_str());
    m_threadLiteInfo = new ThreadLiteInfo;
    m_threadLiteInfo->func = func;
}

CThreadLite::~CThreadLite()
{
    // 此处需要再修改
    if (!isThreadOver())
    {
        join();
    }

    delete m_threadLiteInfo;
}

void CThreadLite::run()
{
    m_threadLiteInfo->func();
}



}   // namespace Infra
