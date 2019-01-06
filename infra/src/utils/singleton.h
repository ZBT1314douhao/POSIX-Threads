#ifndef __INFRA_SRC_OS_SINGLETON_H__
#define __INFRA_SRC_OS_SINGLETON_H__

#include "singleton.h"

#include <cassert>
#include <memory>

#include <pthread.h>

#include "colorfulprint.h"

/// \brief 单例类的声明
/// \code
/// foo.h
/// class Foo
/// {
///     INFRA_SINGLETON_DECLARE(Foo)
/// }
/// \endcode
#define INFRA_SINGLETON_DECLARE(classname)            \
  private:                                            \
    classname();                                      \
    virtual ~classname();                             \
    classname(const classname &) = delete;            \
    classname &operator=(const classname &) = delete; \
                                                      \
  public:                                             \
    static classname &instance();                     \
                                                      \
  private:

/// \brief 单例类的实现
/// \code
/// foo.cpp
/// INFRA_SINGLETON_IMPLEMENT(Foo)
/// \endcode
#define INFRA_SINGLETON_IMPLEMENT(classname)    \
    classname &classname::instance()            \
    {                                           \
        static classname instance;              \
        return instance;                        \
    }

#define INFRA_SINGLETON_DEFAULT_DECLARE(classname) friend class Infra::CSingletonDefault<classname>;

namespace Infra {

/// \brief
/// \note 只能调用默认构造函数的单件模式模板
template<typename T>
class CSingletonDefault
{
public:
    static T& instance()
    {
        pthread_once(&m_once, &CSingletonDefault::init);
        assert(m_instance != NULL);
        return *m_instance;
    }

    static void destory()
    {
        if (m_instance != NULL)
        {
            delete m_instance;
            tracef("delete singleton\n");
        }
    }

private:
    CSingletonDefault();
    ~CSingletonDefault();

    static void init()
    {
        m_instance = new T();
    }

private:
    static pthread_once_t m_once;
    static T *m_instance;
};

/// \note 类模板的静态变量必须在头文件中初始化      
template <typename T>
pthread_once_t CSingletonDefault<T>::m_once = PTHREAD_ONCE_INIT;

template <typename T>
T *CSingletonDefault<T>::m_instance = NULL;

// template <typename T>
// std::shared_ptr<CSingletonDefault<T>> deletor(&CSingletonDefault<T>::instance(), [](CSingletonDefault<T> *) { CSingletonDefault<T>::destory(); });

} // namespace Infra

#endif
