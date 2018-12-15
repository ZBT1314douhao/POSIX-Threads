#ifndef __INFRA_SRC_UTILS_COLORFULPRINT_H__
#define __INFRA_SRC_UTILS_COLORFULPRINT_H__

#include <pthread.h>
#include <cstdio>

#ifndef MODULE_NAME
#define MODULE_NAME     "Unknown"
#endif // !MODULE_NAME

#ifndef MODULE_VERSION
#define MODULE_VERSION  "v1.0"
#endif // !MODULE_VERSION


#ifdef USE_COLORFULPRINT
#define fatalf(fmt, ...)    Infra::Utils::print(Infra::Utils::PRINT_LEVEL_FATAL, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define errorf(fmt, ...)    Infra::Utils::print(Infra::Utils::PRINT_LEVEL_ERROR, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define warnf(fmt, ...)     Infra::Utils::print(Infra::Utils::PRINT_LEVEL_WARN, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define infof(fmt, ...)     Infra::Utils::print(Infra::Utils::PRINT_LEVEL_INFO, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define debugf(fmt, ...)    Infra::Utils::print(Infra::Utils::PRINT_LEVEL_DEBUG, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define tracef(fmt, ...)    Infra::Utils::print(Infra::Utils::PRINT_LEVEL_TRACE, MODULE_NAME, MODULE_VERSION, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define fatalf
#define errorf
#define warnf
#define infof
#define debugf
#define tracef
#endif // USE_COLORFULPRINT

namespace Infra {


namespace Utils
{
    enum PrintLevel
    {
        PRINT_LEVEL_NONE = 0,
        PRINT_LEVEL_FATAL,
        PRINT_LEVEL_ERROR,
        PRINT_LEVEL_WARN,
        PRINT_LEVEL_INFO,
        PRINT_LEVEL_DEBUG,
        PRINT_LEVEL_TRACE,
        PRINT_LEVEL_ALL = PRINT_LEVEL_TRACE,
    };


    bool setPrintLevel(int level);
    int getPrintLevel();

    void print(int level, const char *module, const char *version, 
                const char *func, int line, const char *fmt, ...);


} // namespace Utils
} // namespace Infra

#endif
