
#include "thread_specific_data.h"

#include <assert.h>
#include "utils/colorfulprint.h"
#include "utils/error_check.h"

namespace Infra
{

struct ThreadSpecificData
{
    pthread_key_t key;

};

pthread_once_t CThreadSpecificData::m_key_once = PTHREAD_ONCE_INIT;

CThreadSpecificData::CThreadSpecificData()
{
    m_tsd_info = new ThreadSpecificData();
    pthread_once(&m_key_once, &CThreadSpecificData::init);
}

CThreadSpecificData::~CThreadSpecificData()
{
    pthread_key_delete(m_tsd_info->key);
    if (m_tsd_info != NULL)
    {   
        delete m_tsd_info;
    }
}

bool CThreadSpecificData::init()
{
    int ret = pthread_key_create(&m_tsd_info->key, NULL);
    if (ret != 0)
    {
        fatalf("failed int pthread_key_create, ret = %d\n", ret);
    }
    return (ret == 0);
}

static void CThreadSpecificData::destructor(void *x)
{
    //checked_delete<>
}

bool CThreadSpecificData::get(void *value)
{
    value = pthread_getspecific(m_tsd_info->key);
    if (value == NULL)
    {
        fatalf("failed in pthread_getspecific, ret = NULL\n");
    }
    return (value == NULL);
}

bool CThreadSpecificData::set(const void *value)
{
    return (pthread_setspecific(m_tsd_info->key, value) != 0);
}

}; // namespace Infra
