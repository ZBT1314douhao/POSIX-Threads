// 异步编程举例：闹钟程序
// 程序说明：该程序循环接受用户输入信息，直到出错或输入完毕。用户输入的每行信息中，
// 第一部分时闹钟等待时间（单位：秒），第二部分是闹钟到达时显示的文本消息。
// 本程序为多线程版本
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep头文件
#include <pthread.h>

typedef struct alarm_info
{
    int seconds;       // 等待时间
    char message[128]; // 闹钟显示的消息
} alarm_info_t;

static void * alarm_thread(void *arg);

// 所有线程共享同一个地址空间，因此可以为每个闹铃调用malloc建立新的alarm_info_t结构，并将它传递给新线程

int alarm_create()
{
    printf("please input alarm info: example: seconds message(message length < 128)\n");
    char line[128];
    // 从stdin中最多读取sizeof(line) - 1个字符保存到缓冲区line中
    // 在读取到EOF或newline后停止读取
    // 当到达文件尾无字符可读时返回NULL
    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        exit(0);
    }
    if (strlen(line) <= 1) 
    {
        return -1;
    }
    // 所有线程共享同一个地址空间，因此可以为每个闹铃调用malloc建立新的alarm结构，
    // 并将其传递给新线程
    alarm_info_t *alarm = (alarm_info_t *)malloc(sizeof(alarm_info_t));
    if (alarm == NULL)
    {
        return -1;
    }

    // message最多128个字符
    if (sscanf(line, "%d %128[^\n]", &alarm->seconds, alarm->message) < 2)
    {
        fprintf(stderr, "bad command\n");
        free(alarm);
        alarm = NULL;
    }
    else
    {
        pthread_t tid;
        int status = pthread_create(&tid, NULL, alarm_thread, alarm);
        if (status != 0)
        {
            printf("pthread create status = %d\n", status);
        }
    }

    return 0;
}


void * alarm_thread(void *arg)
{
    alarm_info_t *alarm = (alarm_info_t *)arg;
    // 分离自己以允许当线程终止时立刻回收线程资源
    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        printf("detach thread status = %d\n", status);
    }
    sleep(alarm->seconds);
    printf("alarm arrived: seconds = %d, message = %s\n", alarm->seconds, alarm->message);
    free(alarm);
    alarm = NULL;

    return NULL;

}

int main(int argc, char const *argv[])
{
    while (1)
    {
        alarm_create();
    }
    return 0;
}

