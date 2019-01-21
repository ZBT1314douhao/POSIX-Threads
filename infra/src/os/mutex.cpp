
#include "mutex.h"

#include <assert.h>
#include "thread.h"
#include "utils/colorfulprint.h"

namespace Infra
{

struct MutexInfo
{
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pid_t owner;
};

CMutex::CMutex()
{
    m_mutex_info = new MutexInfo;
    m_mutex_info->owner = 0;

    int ret = pthread_mutexattr_init(&m_mutex_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_mutexattr_init, ret = %d\n", ret);
    }
    pthread_mutex_init(&m_mutex_info->mutex, &m_mutex_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_mutex_init, ret = %d\n", ret);
    }
    ret = pthread_mutexattr_destroy(&m_mutex_info->attr);
    if (ret != 0)
    {
        fatalf("failed in pthread_mutexattr_destroy, ret = %d\n", ret);
    }
}

CMutex::~CMutex()
{
    assert(m_mutex_info->owner == 0);
    int ret = pthread_mutex_destroy(&m_mutex_info->mutex);
    if (ret == EBUSY)
    {
        fatalf("failed in pthread_mutex_destroy, the mutex is currently locked\n");
    }
    else if (ret != 0)
    {
        fatalf("failed in pthread_mutex_destroy, ret = %d\n", ret);
    }
    assert(ret == 0);
    delete m_mutex_info;
}

bool CMutex::lock()
{
    // assert(m_mutex_info->owner == 0);
    if (m_mutex_info->owner != 0)
    {
        errorf("failed in mutex must be unlocked before lock it\n");
        return false;   
    }
    if (pthread_mutex_lock(&m_mutex_info->mutex) == 0)
    {
        m_mutex_info->owner = CThread::getCurrentThreadId();
        tracef("mutex owner = %ld\n", m_mutex_info->owner);
        return true;
    }

    return false;
}

bool CMutex::trylock()
{
    int ret = pthread_mutex_trylock(&m_mutex_info->mutex);
    if (ret == 0)
    {
        m_mutex_info->owner = CThread::getCurrentThreadId();

        return true;
    }
    else if (ret == EBUSY)
    {
        warnf("mutex has locked, owner is %ld, please wait for a moment, ret = EBUSY\n", 
            m_mutex_info->owner);
    }
    else
    {
        warnf("failed in pthread_mutex_trylock ret = %d\n", ret);   
    }
    return false;
}

bool CMutex::unlock()
{
    //assert(m_mutex_info->owner != 0);
    if (m_mutex_info->owner == 0)
    {
        errorf("failed in mutex must be locked before unlock\n");
        return false;
    }

    int ret = pthread_mutex_unlock(&m_mutex_info->mutex);
    if (ret == 0)
    {
        m_mutex_info->owner = 0;

        return true;
    }
    warnf("failed in pthread_mutex_unlock, ret = %d\n", ret);
    return false;
}

CMutex::NativeHandleType CMutex::nativeHandle()
{
    return &m_mutex_info->mutex;
}

bool CMutex::locked() const
{
    return (m_mutex_info->owner != 0);
}

}; // namespace Infra
