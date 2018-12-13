

#ifndef __ZBT_MUTEX_H__
#define __ZBT_MUTEX_H__

#include <pthread.h>
#include <cstdio>

namespace Infra {

struct MutexInfo;

class CMutex
{
    CMutex(const CMutex&);
    CMutex& operator = (const CMutex&);
public:
    CMutex();
    ~CMutex();
    bool lock();
    bool trylock();
    bool unlock();

private:
    pthread_mutex_t m_mutex;
    pthread_mutexattr_t m_mtxattr;
    MutexInfo *m_mutexInfo;
};

class CMutexGuard
{
public:
    explicit CMutexGuard(CMutex& mutex)
    : m_mutex(mutex)
    {
        m_mutex.lock();
        printf("[mutex.h line:33] mutex lock\n");
    }

    ~CMutexGuard()
    {
        m_mutex.unlock();
        printf("[mutex.h line:40] mutex unlock\n");
    }

private:
    CMutex& m_mutex;
};

// 防止出现 CMutexGuard(m_mutex)的写法
#define CMutexGuard(x) static_assert(false, "error, example: CMutexGuard guard(m_mutex)");

}

#endif
