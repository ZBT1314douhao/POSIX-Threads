// 示例：如何等待条件变量

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


typedef struct cond_info
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} cond_info_t;

cond_info_t data = 
{
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    0
};

// hibernation > 2：条件变量等待操作将超时； 
// hibernation == 2：主线程和wait_thread线程发生竞争，并且每次运行结果可能不同；
// hibernation > 2：条件变量等待操作不会超时
int hibernation = 1;    // 休眠时间

void* wait_thread(void *arg)
{
    int status;
    printf("[cond.cpp] wait_thread hibernation = %d\n", hibernation);
    sleep(hibernation);
    if ((status = pthread_mutex_lock(&data.mutex)) != 0)
    {
        err_abort(status, "lock mutex");
    }
    data.value = 1;
    if ((status = pthread_cond_signal(&data.cond)) != 0)
    {
        err_abort(status, "signal condition");
    }
    if ((status = pthread_mutex_unlock(&data.mutex)) != 0)
    {
        err_abort(status, "unlock mutex");
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    int status;
    pthread_t wait_thread_id;
    struct timespec timeout;

    if (argc > 1)
    {
        hibernation = atoi(argv[1]);
    }
    // printf("[cond.cpp] hibernation time = %d\n", hibernation);
    if ((status = pthread_create(&wait_thread_id, NULL, wait_thread, NULL)) != 0)
    {
        err_abort(status, "create counter thread");
    }

    timeout.tv_sec = time(NULL) + 2;
    timeout.tv_nsec = 0;
    if ((status = pthread_mutex_lock(&data.mutex)) != 0)
    {
        err_abort(status, "lock mutex");
    }

    // 在循环中等待条件变量，避免程序错误、多处理器竞争和假唤醒
    while (data.value == 0)
    {
        status = pthread_cond_timedwait(&data.cond, &data.mutex, &timeout);
        printf("[cond.cpp] timed wait status = %d\n", status);
        if (status == ETIMEDOUT)
        {
            printf("[cond.cpp] condition wait timed out\n");
        }
        else if (status != 0)
        {
            err_abort(status, "wait on condition");
        }
    }
    if (data.value != 0)
    {
        printf("[cond.cpp] condition was signaled\n");
    }
    if ((status = pthread_mutex_unlock(&data.mutex)) != 0)
    {
        err_abort(status, "unlock mutex");
    }

    if ((status = pthread_join(wait_thread_id, NULL)) != 0)
    {
        err_abort(status, "join wait thread");
    }


    return 0;
}

