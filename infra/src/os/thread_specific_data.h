#ifndef __INFRA_SRC_OS_THREAD_SPECIFIC_DATA_H__
#define __INFRA_SRC_OS_THREAD_SPECIFIC_DATA_H__

#include <pthread.h>

#include "utils/uncopyable.h"

namespace Infra {

struct ThreadSpecificData;

class CThreadSpecificData
{
    INFRA_DISABLE_COPY(CThreadSpecificData)
public:
    typedef pthread_key_t       NativeType;
    typedef NativeType*         NativeHandleType;

public:
    CThreadSpecificData();
    ~CThreadSpecificData();   
    bool get(void **value);
    bool set(const void *value);
    NativeHandleType nativeHandle();

private:
    bool isValidKey() const;

private:
    ThreadSpecificData *m_tsd_info;
};



}

#endif
