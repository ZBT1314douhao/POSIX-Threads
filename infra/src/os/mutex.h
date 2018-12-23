#ifndef __INFRA_SRC_OS_MUTEX_H__
#define __INFRA_SRC_OS_MUTEX_H__

#include <pthread.h>

namespace Infra {

struct MutexInfo;

class CMutex
{
    CMutex(const CMutex&);
    CMutex& operator = (const CMutex&);
public:
    typedef pthread_mutex_t     NativeType;
    typedef NativeType*         NativeHandleType;

  public:
    CMutex();
    ~CMutex();
    bool lock();
    bool trylock();
    bool unlock();
    bool locked() const;
    NativeHandleType nativeHandle();

  private:
    MutexInfo *m_mutex_info;
};

class CMutexGuard
{
public:
    explicit CMutexGuard(CMutex& mutex)
    : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~CMutexGuard()
    {
        m_mutex.unlock();
    }

private:
    CMutex& m_mutex;
};

// 防止出现 CMutexGuard(m_mutex)的写法
#define CMutexGuard(x) static_assert(false, "error, example: CMutexGuard guard(m_mutex)");

}

#endif
