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

pthread_key_t tsd_key;
pthread_once_t key_once = PTHREAD_ONCE_INIT;

/// \brief 必须保证pthread_key_create对于每个pthread_key_t变量仅仅被调用一次
/// 如果将一个键创建两次，其实是在创建不同的键。第二个键将覆盖第一个，第一个键与任何
/// 线程可能为其设置的值一起将永远地丢失
void once_routine()
{
    printf("initializing pthread key\n");
    int status = pthread_key_create(&tsd_key, NULL);
    if (status != 0)
    {
        err_abort(status, "create key");
    }

}

void* thread_routine(void *arg)
{
    printf("[tsd_once.cpp] %s created\n", arg);
    int status = pthread_once(&key_once, once_routine);
    if (status != 0)
    {
        err_abort(status, "once init");
    }

    tsd_info_t *value = (tsd_info_t *)malloc(sizeof(tsd_info_t));
    if (value == NULL)
    {
        errno_abort("allocate key value");
    }
    status = pthread_setspecific(tsd_key, value);
    if (status != 0)
    {
        err_abort(status, "set tsd");
    }
    printf("[tsd_once.cpp] %s set tsd value %p\n", arg, value);
    value->tid = pthread_self();
    value->string = (char *)arg;

    value = (tsd_info_t *)pthread_getspecific(tsd_key);
    printf("[tsd_once.cpp] %s starting...\n", value->string);
    sleep(2);
    /// 尽管另外的线程把不同的tsd_t结构地址分派给了相同的线程私有数据键，
    /// 但是此处线程私有数据的值和上面保持一样
    value = (tsd_info_t *)pthread_getspecific(tsd_key);
    printf("[tsd_once.cpp] %s done...\n", value->string);

    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    char thread1_info[] = "thread 1";
    char thread2_info[] = "thread 2";
    int status = pthread_create(&thread1, NULL, thread_routine, thread1_info);
    if (status != 0)
    {
        err_abort(status, "create thread 1");
    }

    status = pthread_create(&thread2, NULL, thread_routine, thread2_info);
    if (status != 0)
    {
        err_abort(status, "create thread 2");
    }

    pthread_exit(NULL);
}

