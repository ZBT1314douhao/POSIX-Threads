#ifndef __INFRA_SRC_UTILS_UNCOPYABLE_H__
#define __INFRA_SRC_UTILS_UNCOPYABLE_H__

/// \brief 使用时需要将保证INFRA_DISABLE_COPY处于private访问权限
/// \code
/// class Foo
/// {
///     INFRA_DISABLE_COPY(Foo)
/// };
///
/// \endcode
#define INFRA_DISABLE_COPY(Class)  \
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;

namespace Infra {
namespace Utils
{
/// \brief 构造函数和析构函数设置protected权限，使得仅子类可以调用
/// 拷贝构造函数和拷贝赋值函数设置为private，使得除非子类定义自己的
/// 拷贝构造函数和拷贝赋值函数，否则在子类没有定义时，外面调用者不能通过
/// 这两个手段产生新的子类对象
/// \code
/// class Foo : Infra::UnCopyable
/// {
///     ...
/// }
/// class Foo : public BaseClass, private Infra::UnCopyable
/// {
///     ...
/// }
/// \endcode
/// \note CUnCopyable不含任何数据，符合EBO（空白基类最优化），但EBO一般
/// 在单继承下才可行，编译器通常不会对有多个基类的衍生类执行EBO。
class CUnCopyable
{
protected:
    CUnCopyable();
    ~CUnCopyable();

private:
    CUnCopyable(const CUnCopyable&);
    CUnCopyable& operator=(const CUnCopyable&);
    
};

} // namespace Utils

typedef Utils::CUnCopyable UnCopyable;

} // namespace Infra

#endif
