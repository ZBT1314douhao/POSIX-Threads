#ifndef __INFRA_SRC_UTILS_ERROR_CHECK_H__
#define __INFRA_SRC_UTILS_ERROR_CHECK_H__


namespace Infra {
namespace Utils
{

/// \brief 安全的删除参数所指向的变量或数组
/// \note 指针既可以指向一个变量也可以指向一个数组，下面两个函数并不能
/// 从参数上区分待删除的是单个变量还是数组。因此，调用者必须保证调用了正确的函数。
template<class T> inline void checked_delete(T *x)
{
    /// gcc对不完全类型使用sizeof，会返回0.因此，可以将运行时错误转为编译器错误
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    /// 防止编译器优化及消除未使用sizeof返回值的警告
    (void)sizeof(type_must_be_complete);
    delete x;
}

template<class T> inline void checked_arrary_delete(T *x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete [] x;
}

} // namespace Utils
} // namespace Infra

#endif
