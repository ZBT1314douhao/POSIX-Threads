#ifndef __INFRA_SRC_OS_THREAD_SPECIFIC_DATA_H__
#define __INFRA_SRC_OS_THREAD_SPECIFIC_DATA_H__

#include <pthread.h>

#include "utils/uncopyable.h"

namespace Infra {

struct ThreadSpecificData;

class CThreadSpecificData
{
    INFRA_SINGLETON_DECLARE(CThreadSpecificData)

public:
    typedef pthread_key_t       NativeType;
    typedef NativeType*         NativeHandleType;

public:
    bool get(void *value);
    bool set(const void *value);

private:
    bool init();
    static void destory(void *x);

  private:
    ThreadSpecificData *m_tsd_info;
    static pthread_once_t m_key_once;
    ;
};



}

#endif
