// 非阻塞式互斥量锁

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

// 控制counter_thread线程在更新计数器时保持互斥量时间的长度，
// 增大该值，将增大monitor_thread线程中pthread_mutex_trylock()返回EBUSY的概率
#define SPIN    10000000

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
long g_counter;
time_t g_end_time;

void* counter_thread(void *arg)
{
    int status;
    int spin;

    while (time(NULL) < g_end_time)
    {
        if ((status = pthread_mutex_lock(&g_mutex)) != 0)
        {
            err_abort(status, "mutex locked");
        }
        for (spin = 0; spin < SPIN; ++spin)
        {
            ++g_counter;
        }
        if ((status = pthread_mutex_unlock(&g_mutex)) != 0)
        {
            err_abort(status, "mutex unlocked");
        }
        sleep(1);
    }

    printf("[trylock.cpp] g_counter current value = %#lx\n", g_counter);
    return NULL;
}


void* monitor_thread(void *arg)
{
    int status;
    int misses = 0;

    while (time(NULL) < g_end_time)
    {
        sleep(1);   // 在我的机器上sleep(3)并不会或很难导致EBUSY产生
        status = pthread_mutex_trylock(&g_mutex);
        if (status != EBUSY)
        {
            if (status != 0)
            {
                err_abort(status, "mutex trylock");
            }
            printf("[trylock.cpp] g_counter/SPIN = %ld\n", g_counter / SPIN);
            if ((status = pthread_mutex_unlock(&g_mutex)) != 0)
            {
                err_abort(status, "mutex unlocked");
            }
        }                 
        else
        {
            ++misses;
            printf("[trylock.cpp] trylock return EBUSY, missed = %d\n", misses);
        }
    }

    printf("[trylock.cpp] monitor thread missed update %d times\n", misses);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int status;
    pthread_t counter_thread_id;
    pthread_t monitor_thread_id;

    g_end_time = time(NULL) + 60;   // 运行一分钟
    if ((status = pthread_create(&counter_thread_id, NULL, counter_thread, NULL)) != 0)
    {
        err_abort(status, "create counter thread");
    }
    if ((status = pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL)) != 0)
    {
        err_abort(status, "create monitor thread");
    }
    if ((status = pthread_join(counter_thread_id, NULL)) != 0)
    {
        err_abort(status, "join counter thread");
    }
    if ((status = pthread_join(monitor_thread_id, NULL)) != 0)
    {
        err_abort(status, "join monitor thread");
    }

    return 0;
}

