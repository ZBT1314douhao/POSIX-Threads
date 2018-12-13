        
#include "mutex.h"
#include <assert.h>
#include "thread.h"

namespace Infra {

struct MutexInfo
{
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pid_t owner;
};

CMutex::CMutex()
{
    m_mutexInfo = new MutexInfo;

    m_mutexInfo->owner = 0;
    int ret = pthread_mutexattr_init(&m_mutexInfo->attr);
    if (ret != 0)
    {
        printf("[mutex.cpp line:23] failed in pthread_mutexattr_init, ret = %d\n", ret);
    }
    pthread_mutex_init(&m_mutexInfo->mutex, &m_mutexInfo->attr);
    if (ret != 0)
    {
        printf("[mutex.cpp line:28] failed in pthread_mutex_init, ret = %d\n", ret);
    }
    ret = pthread_mutexattr_destroy(&m_mutexInfo->attr);
    if (ret != 0)
    {
        printf("[mutex.cpp line:33] failed in pthread_mutexattr_destroy, ret = %d\n", ret);
    }
    
    // pthread_mutex_init(&m_mutex, NULL);
}

CMutex::~CMutex()
{
    int ret = pthread_mutex_destroy(&m_mutexInfo->mutex);
    if (ret != 0)
    {
        printf("[mutex.cpp line:44] failed in pthread_mutex_destroy, ret = %d\n", ret);
    } 
    
    // int ret = pthread_mutex_destroy(&m_mutex);
    // assert(ret == 0);   
}

bool CMutex::lock()
{
    assert(m_mutexInfo->owner == 0);
    if (pthread_mutex_lock(&m_mutexInfo->mutex) == 0)
    {
        m_mutexInfo->owner = CThread::getCurrentThreadId();

        return true;
    }

    return false;

    // return (pthread_mutex_lock(&m_mutex) == 0);
}
    
bool CMutex::trylock()
{
    if (pthread_mutex_trylock(&m_mutexInfo->mutex) == 0)
    {
        m_mutexInfo->owner = CThread::getCurrentThreadId();
        
        return true;
    }

    return false;

    // return (pthread_mutex_trylock(&m_mutex) == 0);
}

bool CMutex::unlock()
{
    assert(m_mutexInfo->owner != 0);
    if (pthread_mutex_unlock(&m_mutexInfo->mutex) == 0)
    {
        m_mutexInfo->owner = 0;

        return true;
    }

    return false;

    // return (pthread_mutex_unlock(&m_mutex) == 0);
}

};