#ifndef __INFRA_SRC_OS_THREAD_LOCAL_STORAGE_H__
#define __INFRA_SRC_OS_THREAD_LOCAL_STORAGE_H__

#include <pthread.h>
#include "utils/error_check.h"
#include "utils/uncopyable.h" 

namespace Infra {

/// \brief 线程本地存储
/// \ref muduo base/ThreadLocal.h
template<typename T>
class CThreadLocalStorage
{
public:
    CThreadLocalStorage()
    {
        INFRA_EXPECT_EQ(pthread_key_create(&m_key, &CThreadLocalStorage::destructor), 0);
    }

    ~CThreadLocalStorage()
    { 
        INFRA_EXPECT_EQ(pthread_key_delete(m_key), 0);
    }

    T& value()
    {
        T* tls_value = static_cast<T*>(pthread_getspecific(m_key));
        if (tls_value == nullptr)
        {
            T* new_obj = new T();
            INFRA_EXPECT_EQ(pthread_setspecific(m_key, new_obj), 0);
            tls_value = new_obj;
        }
        return *tls_value;
    }

private:

    static void destructor(void *x)
    {
        T* obj = static_cast<T*>(x);
        Utils::check_delete(obj);
    }

private:
    pthread_key_t m_key;
};

} // namespace Infra

#endif
