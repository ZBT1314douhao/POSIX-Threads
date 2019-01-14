#ifndef __INFRA_SRC_OS_SINGLETON_H__
#define __INFRA_SRC_OS_SINGLETON_H__

#include "singleton.h"

#include <cassert>
#include <memory>
#include <type_traits>

#include <pthread.h>

#include "colorfulprint.h"
#include "error_check.h"

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
    static classname* instance();                     \
                                                      \
  private:

/// \brief 单例类的实现
/// \code
/// foo.cpp
/// INFRA_SINGLETON_IMPLEMENT(Foo)
/// \endcode
#define INFRA_SINGLETON_IMPLEMENT(classname)    \
    classname* classname::instance()            \
    {                                           \
        static classname instance;              \
        return &instance;                        \
    }

#define INFRA_SINGLETON_DEFAULT_DECLARE(classname) friend class Infra::CSingletonDefault<classname>;

namespace Infra {


namespace Detail
{
    /// \brief Substitution failure is not an error (SFINAE)(wikipedia)
    template <typename T>
    struct has_no_destory {
        /// Types "yes" and "no" are guaranteed to have different sizes,
        /// specifically sizeof(yes) == 1 and sizeof(no) == 2.
        typedef char yes[1];
        typedef char no[2];

        template <typename C>
        static yes& test(typename C::no_destory*);

        template <typename>
        static no& test(...);

        /// If the "sizeof" of the result of calling test<T>(nullptr) is equal to sizeof(yes),
        /// the first overload worked and T has a nested type named no_destory.
        static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
    };

    /// use c++11
    template <typename... Ts> using void_t = void;

    template <typename T, typename = void>
    struct has_func_no_destory : std::false_type {};

    template <typename T>
    struct has_func_no_destory<T, void_t<typename T::no_destory>> : std::true_type {};

} // Detail


/// \brief
/// \note 只能调用默认构造函数的单件模式模板
template<typename T>
class CSingletonDefault
{
public:
    static T& instance()
    {
        pthread_once(&m_once, &CSingletonDefault::init);
        assert(m_instance != nullptr);
        return *m_instance;
    }

private:
    CSingletonDefault();
    ~CSingletonDefault();

    static void init()
    {
        m_instance = new T();
        if (!Detail::has_no_destory<T>::value)
        {
            ::atexit(destory);
        }
    }

    static void destory()
    {
        Utils::checked_delete(m_instance);
        tracef("singleton destoryed\n");
        // if (m_instance != nullptr)
        // {
        //     delete m_instance;
        //     m_instance = nullptr;
        //     tracef("delete singleton\n");
        // }
    }

private:
    static pthread_once_t m_once;
    static T *m_instance;
};

/// \note 类模板的静态变量必须在头文件中初始化      
template <typename T>
pthread_once_t CSingletonDefault<T>::m_once = PTHREAD_ONCE_INIT;

template <typename T>
T *CSingletonDefault<T>::m_instance = nullptr;

} // namespace Infra

#endif
