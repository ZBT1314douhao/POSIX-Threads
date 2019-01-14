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


typedef struct tsd_info
{
    pthread_t tid;
    char *string;
} tsd_info_t;

pthread_key_t identity_key;
pthread_mutex_t identity_key_mutex = PTHREAD_MUTEX_INITIALIZER;
long identity_key_counter = 0;

void identity_key_destructor(void *value)
{
    tsd_info_t *info = (tsd_info_t *)value;
    printf("[tsd_destructor.cpp] tid: %d, %s exiting...\n", info->tid, info->string);
    free(value);
    value = NULL;

    int status = pthread_mutex_lock(&identity_key_mutex);
    if (status != 0)
    {
        err_abort(status, "lock key mutex");
    }
    if (--identity_key_counter <= 0)
    {
        status = pthread_key_delete(identity_key);
        if (status != 0)
        {
            err_abort(status, "delete key");
        }
        printf("[tsd_destructor.cpp] key deleted\n");
    }
    printf("[tsd_destructor.cpp] identity_key_counter = %d\n", identity_key_counter);
    status = pthread_mutex_unlock(&identity_key_mutex);
    if (status != 0)
    {
        err_abort(status, "unlock key mutex");
    }
}

void* identity_key_get()
{
    void *value = pthread_getspecific(identity_key);
    if (value == NULL)
    {
        value = malloc(sizeof(tsd_info_t));
        if (value == NULL)
        {
            errno_abort("allocate key value");
        }
        int status = pthread_setspecific(identity_key, (void *)value);
        if (status != 0)
        {
            err_abort(status, "set tsd");
        }
    }

    return value;
}

void* thread_routine(void *arg)
{
    char *thread_info = (char *)arg;
    printf("[tsd_destructor.cpp] thread_info = %s\n", thread_info);
    tsd_info_t *value = (tsd_info_t *)identity_key_get();
    value->tid = pthread_self();
    value->string = (char *)arg;
    printf("[tsd_destructor.cpp] tid: %ul, %s starting...\n", value->tid, value->string);
    sleep(2);

    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    char thread1_info[] = "thread 1";
    char thread2_info[] = "thread 2";

    int status = pthread_key_create(&identity_key, identity_key_destructor);
    if (status != 0)
    {
        err_abort(status, "create key");
    }
    identity_key_counter = 3;
    tsd_info_t *value = (tsd_info_t *)identity_key_get();
    value->tid = pthread_self();
    value->string = (char *)"main thread";

    status = pthread_create(&thread1, NULL, thread_routine, (void *)thread1_info);
    if (status != 0)
    {
        err_abort(status, "create thread 1");
    }

    status = pthread_create(&thread2, NULL, thread_routine, (void *)thread2_info);
    if (status != 0)
    {
        err_abort(status, "create thread 2");
    }
    // sleep(10);
    pthread_exit(NULL);
}

