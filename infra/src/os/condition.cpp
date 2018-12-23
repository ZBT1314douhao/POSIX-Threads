
#include "condition.h"

#include <errno.h>
#include <sys/time.h>
#include <cassert>
#include "utils/colorfulprint.h"

namespace Infra
{

struct ConditionInfo
{
    pthread_cond_t cond;
    pthread_condattr_t attr;
};

CCondition::CCondition()
{
    m_cv_info = new ConditionInfo;
    // 状态初始化

    int ret = pthread_condattr_init(&m_cv_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_condattr_init, ret = %d\n", ret);
    }
    ret = pthread_cond_init(&m_cv_info->cond, &m_cv_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_cond_init, ret = %d\n", ret);
    }
    ret = pthread_condattr_destroy(&m_cv_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_condattr_destroy, ret = %d\n", ret);
    }
}

CCondition::~CCondition()
{
    int ret = pthread_cond_destroy(&m_cv_info->cond);
    if (ret == EBUSY)
    {
        warnf("failed in some threads are currently waiting on cond\n");
    }
    else if (ret != 0)
    {
        fatalf("failed in pthread_cond_destroy, ret = %d\n", ret);
    }
    assert(ret == 0);
    delete m_cv_info;
}

bool CCondition::signal()
{
    // 重启所有等待该条件变量的线程中的一个，如果没有线程等待该条件变量，则什么都不做
    // 如果不止一个线程在等待该条件变量，则只有一个线程可以被唤醒，但是被唤醒的是哪个
    // 线程则是未指定的
    int ret = pthread_cond_signal(&m_cv_info->cond);
    if (ret != 0)
    {
        errorf("failed in pthread_cond_signal, ret = %d\n", ret);

        return false;
    }

    return true;
}

bool CCondition::broadcast()
{
    // 重启所有等待该条件变量的线程，如果没有线程等待该条件变量，则什么都不做
    int ret = pthread_cond_broadcast(&m_cv_info->cond);
    if (ret != 0)
    {
        errorf("failed in pthread_cond_broadcast, ret = %d\n", ret);

        return false;
    }

    return true;
}

bool CCondition::wait(CMutex& mutex)
{
    if (!mutex.locked())
    {
        errorf("the mutex must be locked before wait\n");
        return false;
    }
    // 原子的解锁互斥锁等待条件变量被通知，在条件变量被通知之前，线程被挂起，不消耗
    // 任何CPU时间，在进入该函数之前，互斥量必须已经加锁
    int ret = pthread_cond_wait(&m_cv_info->cond, mutex.nativeHandle());
    if (ret != 0)
    {
        errorf("failed in pthread_cond_wait, ret = %d\n", ret);

        return false;
    }

    return true;
}

bool CCondition::timedwait(CMutex& mutex, int seconds)
{
    if (!mutex.locked())
    {
        errorf("the mutex must be locked before timedwait\n");
        return false;
    }
    struct timeval now;
    gettimeofday(&now, NULL);
    struct timespec timeout;
    seconds = (seconds <= 0) ? 1 : seconds;
    timeout.tv_sec = now.tv_sec + seconds;
    timeout.tv_nsec = now.tv_usec * 1000;
    // 功能同pthread_cond_wait()，区别在于如果在指定的超时时间内没有收到信号，
    // 互斥锁将重新获取该函数返回ETIMEDOUT
    // 第三个参数abstime为0对应00:00:00 GMT, January 1, 1970
    int ret = pthread_cond_timedwait(&m_cv_info->cond, mutex.nativeHandle(), &timeout);
    if (ret == ETIMEDOUT) 
    {
        warnf("after wait %d seconds, timeout\n", seconds);
    } 
    else if (ret != 0)
    {
        errorf("failed in pthread_cond_timedwait, ret = %d\n", ret);
    }

    return (ret == 0);
}

CCondition::NativeHandleType CCondition::nativeHandle()
{
    return &m_cv_info->cond;
}

}; // namespace Infra
