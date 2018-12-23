// alarm_thread.cpp的改进版本，在一个alarmserver线程中处理多个闹钟请求
// 缺点：响应性能不够，一旦alarm_thread线程从列表中收到了一个闹铃请求，就会进入睡眠直到闹钟到期。
// 当它发现列表中没有闹铃请求时也会睡眠一秒，以允许主线程接收新的用户请求。
// 当alarm_thread线程睡眠时，它就不能注意到由主线程添加到请求列表中的任何闹铃请求，直到它从睡眠中返回
// 例如：先设定一个20s的闹钟，再设定一个10s的闹钟，尽管10s的闹钟会提前过期，但是只有当20s的闹钟过期时，
// 才能看到10s的闹钟打印的过期信息

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>     // sleep头文件
#include <pthread.h>

#define err_abort(code, text)   do {\
        fprintf(stderr, "%s at \"%s\":%d: %s\n", text, __FILE__, __LINE__, strerror(code));\
        abort();\
        } while (0)

#define errno_abort(text)   do {\
        fprintf(stderr, "%s at \"%s\":%d: %s\n", text, __FILE__, __LINE__, strerror(errno));\
        abort(); \
        } while (0)

typedef struct alarm_info
{
    struct alarm_info *link;
    int seconds;       // 等待时间
    time_t time;       // 表示从UNIX纪元（1970年1月1日 00:00）开始到闹铃时的秒数
    char message[128]; // 闹钟显示的消息
}alarm_info_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
// 主线程和alarm_thread线程共享数据，需要加锁
alarm_info_t *alarm_list = NULL;

static void * alarm_thread(void *arg);

void * alarm_thread(void *arg)
{
    // time_t now;
    int status;
    alarm_info_t *alarm = NULL;
    int sleep_time;

    while (1)
    {
        if ((status = pthread_mutex_lock(&alarm_mutex)) != 0)
        {
            err_abort(status, "mutex locked");
        }
        alarm = alarm_list;
        // 如果没有新的请求，alarm_thread线程将阻塞自己一小会，释放互斥量
        // 让主线程能够运行以添加新的闹铃请求
        if (alarm == NULL)
        {
            sleep_time = 1;
            //printf("[alarm_mutex.cpp] no new alarm request, sleep 1s\n");
        }
        else
        {
            // 发现闹铃请求后，将其(alarm)从闹铃列表中删除
            alarm_list = alarm->link;
            // now = time(NULL);
            // sleep_time = (alarm->time <= now)?  : alarm->time - now;
            sleep_time = alarm->time - time(NULL);
            printf("[alarm_mutex.cpp] waiting alarm->time = %d, left sleep_time = %d, alarm->message = %s\n",
                    alarm->time, sleep_time, alarm->message);
        }
        if ((status = pthread_mutex_unlock(&alarm_mutex)) != 0)
        {
            err_abort(status, "mutex unlocked");
        }

        // sched_yield()将处理器交给另一个等待运行的线程，如果没有就绪的线程，则立即返回
        // 即，如果有等待处理的用户输入，则主线程运行；否则该函数立即返回
        (sleep_time > 0) ? sleep(sleep_time) : sched_yield();

        // 闹铃请求处理完毕，释放资源，准备处理下一个闹铃请求
        if (alarm != NULL)
        {
            printf("[alarm_mutex.cpp] alarm arrived, alarm->seconds = %d, alarm->message = %s\n",
                    alarm->seconds, alarm->message);
            free(alarm);
            alarm = NULL;
        }
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    char line[128];
    // last指向前一个节点的link指针或者指向列表头指针
    // next指针指向当前节点
    alarm_info_t *alarm, **last, *next;
    pthread_t tid;

    int status = pthread_create(&tid, NULL, alarm_thread, NULL);
    if (status != 0)
    {
        err_abort(status, "pthread_create alarm_thread");
    }

    while (1)
    {
        printf("[alarm_mutex.cpp] pelase input alarm info: seconds messages\n");
        // 从stdin中最多读取sizeof(line) - 1个字符保存到缓冲区line中
        // 在读取到EOF或newline后停止读取
        // 当到达文件尾无字符可读时返回NULL
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            exit(0);
        }
        if (strlen(line) <= 1) 
        {
            continue;
        }
        alarm = (alarm_info_t *)malloc(sizeof(alarm_info_t));
        if (alarm == NULL)
        {
            errno_abort("malloc alarm_info_t");
        }

        // message最多128个字符
        if (sscanf(line, "%d %128[^\n]", &alarm->seconds, alarm->message) < 2)
        {
            fprintf(stderr, "bad command, please input again\n");
            free(alarm);
            alarm = NULL;
        }
        else
        {
            if ((status = pthread_mutex_lock(&alarm_mutex)) != 0)
            {
                err_abort(status, "mutex locked");
            }
            alarm->time = time(NULL) + alarm->seconds;

            // 根据超时时间将新的闹铃插入闹铃列表
            last = &alarm_list;
            next = *last;
            while (next != NULL)
            {
                if (next->time >= alarm->time)
                {
                    alarm->link = next;
                    *last = alarm;
                    break;
                }
                last = &next->link;
                next = next->link;
            }
            if (next == NULL)
            {
                printf("[alarm_mutex.cpp] next == NULL\n");
                *last = alarm;
                alarm->link = NULL;
            }

            printf("[alarm_mutex.cpp] list elements = ");
            for (next = alarm_list; next != NULL; next = next->link)
            {
                printf("absolute time = %d, wait time = %d, alarm message = \"%s\"\n", 
                        next->time, next->time - time(NULL), next->message);
            }
            printf("[alarm_mutex.cpp] list end\n");

            if ((status = pthread_mutex_unlock(&alarm_mutex)) != 0)
            {
                err_abort(status, "mutex unlocked");
            }
        }
    }

    return 0;
}

