#ifndef __INFRA_SRC_OS_CONDITION_H__
#define __INFRA_SRC_OS_CONDITION_H__

#include <pthread.h>
#include "mutex.h"

namespace Infra {

struct ConditionInfo;

class CCondition
{
    CCondition(const CCondition&);
    CCondition& operator = (const CCondition&);
public:
    typedef pthread_cond_t      NativeType;
    typedef NativeType*         NativeHandleType;
public:
    CCondition();
    ~CCondition();
    bool signal();
    bool broadcast();
    bool wait(CMutex& mutex);
    bool timedwait(CMutex& mutex, int seconds);
    NativeHandleType nativeHandle();

  private:
    ConditionInfo *m_cv_info;
};


}

#endif
