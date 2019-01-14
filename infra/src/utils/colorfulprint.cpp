
#include "colorfulprint.h"

#include <string.h>
#include <cstdio>
#include <cstdarg>

namespace Infra {
namespace Utils {

#define NONE        "\e[0m"
#define BLACK       "\e[0;30m"
#define L_BLACK     "\e[1;30m"
#define RED         "\e[0;31m"
#define L_RED       "\e[1;31m"
#define GREEN       "\e[0;32m"
#define L_GREEN     "\e[1;32m"
#define BROWN       "\e[0;33m"
#define YELLOW      "\e[1;33m"
#define BLUE        "\e[0;34m"
#define L_BLUE      "\e[1;34m"
#define PURPLE      "\e[0;35m"
#define L_PURPLE    "\e[1;35m"
#define CYAN        "\e[0;36m"
#define L_CYAN      "\e[1;36m"
#define GRAY        "\e[0;37m"
#define WHITE       "\e[1;37m"


static int currentLevel = PRINT_LEVEL_WARN;
static const char *color[] =
{
    BLACK, RED, PURPLE, YELLOW, GREEN, CYAN, WHITE,
};

bool setPrintLevel(int level)
{
    if (level < PRINT_LEVEL_NONE || level > PRINT_LEVEL_ALL)
    {
        return false;
    }
    currentLevel = level;

    return true;
}

int getPrintLevel()
{
    return currentLevel;
}

void print(int level, const char *module, const char *version, const char *filename, const char *func, int line, const char *fmt, ...)
{
    if (level > currentLevel || module == nullptr || version == nullptr || func == nullptr)
    {
        //printf("[colorfulprint.cpp] level = %d, currentLevel = %d\n", level, currentLevel);
        return;
    }
    const char *file = NULL;
    const char *slash = strrchr(filename, '/'); // builtin function
    if (slash)
    {
        file = slash + 1;
    }
    printf("%s[%s-%s] [%s %s line:%d] ", color[level], module, version, (file == NULL) ? "" : file, func, line);
    va_list va;
    // void va_start(va_list ap, last); last为可变参数列表之前的最后一个参数名称
    // 每个va_start()调用必须有对应的va_end()
    va_start(va, fmt);
    vprintf(fmt, va);   // 返回值代表已打印字符的数量，不包含null
    va_end(va);
    printf(NONE);
}


} // Utils    
} // Infra 


