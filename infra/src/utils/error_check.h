#ifndef __INFRA_SRC_UTILS_ERROR_CHECK_H__
#define __INFRA_SRC_UTILS_ERROR_CHECK_H__

/// \ref gooletest
#define INFRA_EXPECT_TRUE(cond)                         \
    do {                                                \
    if (!(cond))                                        \
    {                                                   \
        fprintf(stderr, "check failed: %s\n", #cond);   \
        exit(1);                                        \
    }                                                   \
    } while (0)

#define INFRA_EXPECT_FALSE(cond)  INFRA_EXPECT_TRUE(!(cond))

#define INFRA_EXPECT_OP(op, val1, val2)                                     \
    do {                                                                    \
        if (!((val1) op (val2)))                                            \
        {                                                                   \
            fprintf(stderr, "check failed: %s %s %s\n", #val1, #op, #val2); \
            exit(1);                                                        \
        }                                                                   \
    } while (0)

#define INFRA_EXPECT_EQ(val1, val2)  INFRA_EXPECT_OP(==, val1, val2)
#define INFRA_EXPECT_NE(val1, val2)  INFRA_EXPECT_OP(!=, val1, val2)
#define INFRA_EXPECT_GT(val1, val2)  INFRA_EXPECT_OP(>, val1, val2)
#define INFRA_EXPECT_LT(val1, val2)  INFRA_EXPECT_OP(<, val1, val2)

namespace Infra {

namespace Utils
{

/// \brief 安全的删除参数所指向的变量或数组，参考boost
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
