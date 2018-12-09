// 异步编程举例：闹钟程序
// 程序说明：该程序循环接受用户输入信息，直到出错或输入完毕。用户输入的每行信息中，
// 第一部分时闹钟等待时间（单位：秒），第二部分是闹钟到达时显示的文本消息。
// 本程序为基本的异步版本，为每个命令使用fork调用生成一个子进程，在子进程中异步调用sleep函数
// 而父进程则继续运行。
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>     // sleep头文件
#include <string.h>


int main(int argc, char const *argv[])
{
    // 每个闹铃有一个从主进程拷贝的独立地址空间，因此可以将闹铃时间和消息保存在局部变量中，
    // 一旦建立了子进程，父进程就可以改变这些变量而不会影响闹铃子进程。
    pid_t pid;
    int status;
    int seconds;
    char line[128];
    char message[64];

    while (1)
    {
        printf("[alarm_fork.cpp] please input alarm info:\n\
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
            // fork调用返回成功，则建立子进程
            pid = fork();
            if (pid == (pid_t)-1)   // fork失败，子进程未创建成功
            {
                abort();
            }
            printf("[alarm_fork.cpp] after fork pid = %ld\n", pid);
            if (pid == (pid_t)0)
            {
                sleep(seconds);
                printf("[alarm_base.cpp] sleep time arrived, seconds = %d, message = %s\n",
                       seconds, message);
                exit(0);
            }  
            else
            {
                do
                {
                    // 在每个命令之后循环调用waitpid来回收所有结束的子进程，
                    // 通过设置WNOHANG使得父进程不必挂起等待子进程的结束
                    // 如果有子进程终止，该函数调用回收子进程的资源；
                    // 如果没有子进程终止，该函数立即返回pid = 0,
                    // 父进程则继续回收终止的子进程直到没有子进程终止
                    pid = waitpid((pid_t)-1, NULL, WNOHANG);
                    printf("[alarm_fork.cpp] waitpid = %ld\n", pid);
                    if (pid == (pid_t)-1)
                    {
                        abort();
                    }
                } while (pid != (pid_t)(0));
            }
            
        }
    }

    return 0;
}

