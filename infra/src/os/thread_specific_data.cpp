
#include "thread_specific_data.h"

#include <cassert>
#include <cerrno>
#include "cstdlib"
#include "utils/colorfulprint.h"
#include "utils/error_check.h"

namespace Infra {

struct ThreadSpecificData
{
    pthread_key_t key;
    bool valid;
};

CThreadSpecificData::CThreadSpecificData()
{
    m_tsd_info = new ThreadSpecificData();
    int ret = pthread_key_create(&m_tsd_info->key, NULL);
    if (ret != 0)
    {
        fatalf("failed in pthread_key_create, ret = %d\n", ret);
    }
    m_tsd_info->valid = (ret == 0);
}

CThreadSpecificData::~CThreadSpecificData()
{
    if (isValidKey())
    {
        int ret = pthread_key_delete(m_tsd_info->key);
        if (ret == EINVAL)
        {
            errorf("key is not a valid, allocated TSD key\n");
        }
        else if (ret != 0)
        {
            errorf("failed in pthread_key_delete, ret = %d\n", ret);
        }
    }
    delete m_tsd_info;
    m_tsd_info = NULL;
}

bool CThreadSpecificData::get(void **value)
{   
    if (!isValidKey())
    {
        return false;
    }

    *value = pthread_getspecific(m_tsd_info->key);
    if (*value != nullptr)
    {
        infof("value = %d(addr = %p)\n", *value, value);
    }

    return (value != nullptr);
}

bool CThreadSpecificData::set(const void *value)
{
    if (!isValidKey())
    {
        return false;
    }

    infof("before value = %lu\n", value);
    int ret = pthread_setspecific(m_tsd_info->key, value);
    if (ret != 0)
    {
        errorf("failed in pthread_setspecific, ret = %d\n", ret);
    }

    return (ret == 0);
}

CThreadSpecificData::NativeHandleType CThreadSpecificData::nativeHandle()
{
    return &m_tsd_info->key;
}

bool CThreadSpecificData::isValidKey() const
{
    return m_tsd_info->valid;
}

}; // namespace Infra
