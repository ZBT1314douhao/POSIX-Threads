// 试锁-回退：在锁住某个集合中的第一个互斥量后，使用pthread_mutex_trylock()来加锁
// 集合中的其他互斥量，如果失败则将集合中所有已加锁互斥量释放，并重新加锁

// 在我的机器上，要使 g_use_sched_yield != 0才可以观察到死锁现象， g_use_sched_yield < 0的时候，程序运行耗时非常长
// ./challenge 0 1可以看到死锁现象， ./challenge 1 1可以看到试锁-回退现象

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


#define ITERATIONS    10

pthread_mutex_t g_mutex[3] = 
{
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};
int g_use_backoff = 1;      // 是否使用试锁-回退算法，如果不使用将导致死锁
int g_use_sched_yield = 0;   //  = 0: no yield, > 0: sched_yield(), < 0: sleep(1);

void* lock_forward(void *arg)
{
    int i;
    int iterate;
    int backoffs;
    int status;

    
    for(iterate = 0; iterate < ITERATIONS; iterate++)
    {
        printf("[backoff.cpp] lock_forward iterate = %d\n", iterate);
        backoffs = 0;
        for (i = 0; i < 3; ++i)
        {
            if (i == 0)
            {
                status = pthread_mutex_lock(&g_mutex[i]);
                if (status != 0)
                {
                    err_abort(status, "first lock");
                }
                printf("[backoff.cpp] forward locker got %d\n", i);
            }
            else
            {
                status = (g_use_backoff == 0) ? pthread_mutex_lock(&g_mutex[i]) : pthread_mutex_trylock(&g_mutex[i]);
                if (status == EBUSY)
                {
                    ++backoffs;
                    printf("[backoff.cpp] forward locker backing off at %d\n", i);
                    for (; i >= 0; i--)
                    {
                        if ((status = pthread_mutex_unlock(&g_mutex[i])) != 0)
                        {
                            err_abort(status, "back off");
                        }
                    }
                }
                else
                {
                    if (status != 0)
                    {
                        err_abort(status, "lock mutex");
                    }
                    printf("[backoff.cpp] forward locker got %d\n", i);
                }

            }

            if (g_use_sched_yield)
            {
                (g_use_sched_yield > 0) ? sched_yield() : sleep(1);
            }
        }

        printf("[backoff.cpp] lock forward got all locks, %d backoffs\n", backoffs);
        pthread_mutex_unlock(&g_mutex[2]);
        pthread_mutex_unlock(&g_mutex[1]);
        pthread_mutex_unlock(&g_mutex[0]);
        sched_yield();
    }

    return NULL;
}


void* lock_backwrad(void *arg)
{
    int i;
    int iterate;
    int backoffs;
    int status;

    for(iterate = 0; iterate < ITERATIONS; ++iterate)
    {
        printf("[backoff.cpp] lock_backwrad iterate = %d\n", iterate);
        backoffs = 0;
        for (i = 2; i >= 0; i--)
        {
            if (i == 2)
            {
                status = pthread_mutex_lock(&g_mutex[i]);
                if (status != 0)
                {
                    err_abort(status, "first lock");
                }
                printf("[backoff.cpp] backwrad locker got %d\n", i);

            }
            else
            {
                status = (g_use_backoff == 0) ? pthread_mutex_lock(&g_mutex[i]) : pthread_mutex_trylock(&g_mutex[i]);
                if (status == EBUSY)
                {
                    ++backoffs;
                    printf("[backoff.cpp] backwrad locker backing off at %d\n", i);
                    for (; i < 3; i++)
                    {
                        if ((status = pthread_mutex_unlock(&g_mutex[i])) != 0)
                        {
                            err_abort(status, "back off");
                        }
                    }
                }
                else
                {
                    if (status != 0)
                    {
                        err_abort(status, "lock mutex");
                    }
                    printf("[backoff.cpp] backwrad locker got %d\n", i);
                }

            }

            if (g_use_sched_yield)
            {
                (g_use_sched_yield > 0) ? sched_yield() : sleep(1);
            }
        }

        printf("[backoff.cpp] lock forward got all locks, %d backoffs\n", backoffs);
        pthread_mutex_unlock(&g_mutex[0]);
        pthread_mutex_unlock(&g_mutex[1]);
        pthread_mutex_unlock(&g_mutex[2]);
        sched_yield();
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    int status;
    pthread_t lock_forward_id;
    pthread_t lock_backwrad_id;

    if (argc > 1)
    {
        g_use_backoff = atoi(argv[1]);
    }
    if (argc > 2)
    {
        g_use_sched_yield = atoi(argv[2]);
    }
    printf("[backoff.cpp] g_back_off = %d, g_use_sched_yield = %d\n", g_use_backoff, g_use_sched_yield);
    if ((status = pthread_create(&lock_forward_id, NULL, lock_forward, NULL)) != 0)
    {
        err_abort(status, "create lock_forward thread");
    }
    if ((status = pthread_create(&lock_backwrad_id, NULL, lock_backwrad, NULL)) != 0)
    {
        err_abort(status, "create lock_backwrad thread");
    }
    sleep(2);
    if ((status = pthread_join(lock_forward_id, NULL)) != 0)
    {
        err_abort(status, "join lock_forward thread");
    }
    if ((status = pthread_join(lock_backwrad_id, NULL)) != 0)
    {
        err_abort(status, "join lock_backwrad thread");
    }

    // printf("[backoff.cpp] pthread_exit\n");
    pthread_exit(NULL);
}

