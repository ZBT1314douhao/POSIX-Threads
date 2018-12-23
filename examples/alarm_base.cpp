// 同步编程举例：闹钟程序
// 程序说明：该程序循环接受用户输入信息，直到出错或输入完毕。用户输入的每行信息中，
// 第一部分时闹钟等待时间（单位：秒），第二部分是闹钟到达时显示的文本消息。
// 本程序为基本的同步版本，一次只能处理一个闹钟请求，实际上是把希望异步处理的工作
// 同步处理了

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep头文件
#include <pthread.h>

int main(int argc, char const *argv[])
{
    int seconds;
    char line[128];
    char message[64];

    while (1)
    {
        printf("[alarm_base.cpp] please input alarm info:\n\
        example: seconds message(message length < 128)\n\
        instance: 2 wait for 2s\n");
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            exit(0);
        }
        if (strlen(line) < 1)
        {
            continue;
        }
        if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2)
        {
            printf("[alarm_base.cpp] bad command\n");
        }
        else
        {
            sleep(seconds);
            printf("[alarm_base.cpp] sleep time arrived, seconds = %d, message = %s\n",
                    seconds, message);
        }
    }

    return 0;
}

