# POSIX多线程程序设计：线程

[TOC]

## 术语和定义

**主线程（初始线程）：**当程序执行时，首先运行main函数，在线程代码中，这个特殊的执行流被称为“初始线程”或“主线程”。可以在主线程中做任何可以在普通线程中做的事情。

>初始线程的特殊性在于pthreads在main函数返回阶段保留了传统unix进程行为。即，进程结束时不会等待其他线程结束。
>
>主线程的调用参数（argc，argv）和其他普通线程（void *）也不一样。
>
>从main函数中返回将导致进程终止，也将导致进程中的所有线程终止。可以在main函数中调用pthread_exit()，这样初始线程终止时其他线程继续执行，进程就必须等待所有线程结束后才返回。
>
>在大多数系统中，初始线程运行在默认进程堆栈上，该堆栈可以增长到足够的尺寸；而在某些实现中，普通线程的堆栈空间是受限的，如果程序堆栈溢出，则程序会因段错误或总线错误而失败。





## 线程的生命周期

任意时刻，线程处于**就绪、运行、阻塞、终止**这四个基本状态之一，在具体的实现中，可能会有其他附加的状态，这些附加状态区别于进入四个基本状态的原因。

| 线程状态           | 含义                                                         |
| ------------------ | ------------------------------------------------------------ |
| 就绪（ready）      | 线程能够运行，但在等待可用的处理器。可能刚刚启动（被创建），或刚刚从阻塞中恢复，或被其他线程抢占。 |
| 运行（running）    | 线程正在运行。当处理器选中一个就绪线程执行时，被选中线程变为运行态。在多处理器系统中，可能会有多个线程同时处于运行状态。 |
| 阻塞（blocked）    | 线程由于等待处理器外的其他条件无法运行，如等待某个条件变量，试图加锁一个已经被锁住的互斥量或执行一个无法立即完成的I/O操作。 |
| 终止（Terminated） | 线程从启动函数中返回，或调用pthread_exit()，或被取消，终止自己并完成所有资源的清理工作。不是被分离，也不是被连接，一旦线程被分离或连接，它就可以被收回。 |





## 创建线程

进程的初始线程随着进程的创建而创建，其他线程需要显式的函数调用（phtread_create()）建立。在新线程建立后，它处于就绪态。

```c++
// \brief pthread_create()在调用进程中启动一个新线程。
// thread: 存储新线程的ID，创建后可以通过pthread_self()获取线程ID。
// attr: 线程属性，可以通过pthread_attr_init()相关函数进行初始化，也可以使用NULL使用默认属性进行初始化。
// start_routine: 线程体，新线程通过调用start_routine()开始执行。
// arg: 要向新创建的线程传递的参数。
// return: 成功时返回0，失败时返回一个errorno，此时*thread的内容是未定义的。
int pthread_create(pthread_t *thread, 
                   const pthread_attr_t *attr, 
                   void *(*start_routine) (void *), 
                   void *arg);
```

> 在当前线程从函数pthread_create()中返回以及新线程被调度执行之间不存在同步关系。即，新线程可能在当前线程从pthread_create()返回之前就运行甚至运行完毕了。



### 线程属性

```c++
// \brief 使用默认的值初始化线程属性对象。默认值如下
// Detach state        = PTHREAD_CREATE_JOINABLE
// Scope               = PTHREAD_SCOPE_SYSTEM
// Inherit scheduler   = PTHREAD_INHERIT_SCHED
// Scheduling policy   = SCHED_OTHER
// Scheduling priority = 0
// 以下几个很少需要或最好不要自行设置
// Guard size          = 4096 bytes		
// Stack address       = 0x40196000
// Stack size          = 0x201000 bytes
int pthread_attr_init(pthread_attr_t *attr);
// \brief 当线程属性对象不再需要的时候，可以通过该函数销毁。销毁一个线程属性对象对使用该对象创建的线程没有任何影响
int pthread_attr_destroy(pthread_attr_t *attr);
```



#### 分离状态

```c++
// \brief 设置使用attr属性创建的线程的的分离状态
// detachstate可取的值：PTHREAD_CREATE_DETACHED 或 
// PTHREAD_CREATE_JOINABLE（默认值）
int pthread_attr_setdetachstate(pthread_attr_t *attr, 
                                int detachstate);
```

* PTHREAD_CREATE_JOINABLE： 另一个线程可以调用pthread_join()等待线程终止并获取其退出状态。只有当一个终止的joinable线程已经被连接，其资源才会由释放还给系统。一个以joinable状态创建的线程，最后应该要么由pthread_join()连接，要么用pthread_detach()分离线程，否则资源线程将一直处于终止态，资源无法被回收。
* PTHREAD_CREATE_DETACHED： 则当一个detached线程终止时，其资源自动返还给系统，不能连接一个处于detached状态的线程。

>一旦线程被回收，线程ID就无效了，不能再连接它，取消它或者执行其他任何操作。因为，终止线程ID可能会被分配给新的线程，使用该ID调用pthread_cancel()就会取消一个不同的线程。



#### 竞争范围

```c++
// \brief 设置使用attr属性创建的线程的的竞争范围
// scope可取的值：PTHREAD_SCOPE_SYSTEM 或 PTHREAD_SCOPE_PROCESS
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
```

* PTHREAD_SCOPE_SYSTEM： 线程和系统上的在同一个调度分配领域的所有进程中的所有其他线程竞争资源。通常表明一个用户空间线程直接绑定到单个内核调度实体。
* PTHREAD_SCOPE_PROCESS： 线程和同一进程中的所有其他线程竞争资源。（Linux不支持）

>在支持多个竞争范围的系统上，为了使pthread_attr_setscope()在调用pthread_create()时生效，必须用pthread_attr_setinheritsched()设置继承调度器属性为PTHREAD_EXPLICIT_SCHED。



#### 继承调度器

```c++
// \brief 决定使用attr属性创建的线程是否从调用线程继承调度器属性
// inheritsched可能的取值： PTHREAD_INHERIT_SCHED(默认值) 或 
// PTHREAD_EXPLICIT_SCHED
int pthread_attr_setinheritsched(pthread_attr_t *attr, 
                                 int inheritsched);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, 
                                 int *inheritsched);

```



#### 调度策略

```c++
// \brief 设置使用attr属性创建的线程的调度策略
// policy可能的取值： SCHED_FIFO, SCHED_RR, SCHED_OTHER,
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, 
                                int *policy);
```

* SCHED_FIFO: First in-first out 

    仅可用于静态优先级大于0的时候，这意味着当一个采用SCHED_FIFO策略的线程变为可运行状态时，它将立即抢占其他非实时（SCHED_OTHER等）优先级线程。

    * 当一个运行着的SCHED_FIFO线程被另一个更高优先级的线程抢占后，只有在其他所有更高优先级的线程阻塞后才能恢复执行。
    * 当一个阻塞的SCHED_FIFO线程变得可运行时，将会被插入优先级列表的尾部。
    * 当动态改变一个SCHED_FIFO线程的优先级时，如果线程优先级被提升，它将会被放到优先级表的尾部；如果线程优先级被降低，它将会被放在优先级表的头部。
    * 调用sched_yield()将导致其优先级被放到优先级表的尾部。

    > 一个SCHED_FIFO线程会一直运行直到它被一个I/O请求阻塞，或被更高优先级的线程抢占，或它自己主动调用sched_yield()

* SCHED_RR: Round-robin

    SCHED_RR是SCHED_FIFO的简单增强版，除了具备SCHED_FIFO的以上属性外，还允许每个线程运行一个最大的时间。如果一个SCHED_RR运行了大于或等于最大时间后，其将被放到它的优先级表的尾部。

    > 最大时间可以通过sched_rr_get_interval()获取

* SCHED_OTHER: 默认的Linux分时调度

    SCHED_OTHER仅可在静态优先级为0的时候使用，是为不需要特殊的实时机制而设计的标准Linux分时调度器。一个将要运行的线程会从静态优先级为0的优先级表中选择出来，选择标准基于一个仅作用域此优先级表的动态优先级，而这个动态优先级是基于nice值的。nice值的范围一般是-20（高优先级）到19（低优先级），默认值为0，nice值越大，获得的处理器时间越长。

    >根据Linux的版本不同，可能非实时优先级调度还有 SCHED_BATCH(Since Linux 2.6.16.) 和 SCHED_IDLE(Since Linux 2.6.23.) ，可能的实时优先级调度还有SCHED_DEADLINE( Since version 3.14)


#### 调度优先级

```c++
// \brief 设置调度属性
// struct sched_param 
// {
//     int sched_priority;     /* Scheduling priority */
// };
// sched_priority的取值范围： 1（低）到99（高），非实时进程的优先级为0，可看
// 出实时进程永远比非实时线程的优先级高。
// sched_get_priority_min() 和 sched_get_priority_max()可以获取到支的
// 优先级范围
int pthread_attr_setschedparam(pthread_attr_t *attr,
                               const struct sched_param *param);
int pthread_attr_getschedparam(const pthread_attr_t *attr,
                               struct sched_param *param);
```





## 终止线程

线程可以通过以下方式终止：

* 线程调用pthread_exit()，指定一个退出状态值并且该退出状态的值可以被同进程中的其他线程通过调用pthread_join()获取到。
* 从start_routine()中返回，相当于调用pthread_exit()，返回值为return语句的返回值。
* 被取消（被另一个线程调用pthread_cancel()）
* 进程中的任一线程调用exit()，或主线程返回(return)，将导致进程中的所有线程终止。



#### 退出线程

```c++
// \breif 终止调用线程，并通过retval返回线程退出状态（如果线程是joinable），同
// 一进程中的其他线程可以通过调用pthread_join()获取到该退出状态
// 当线程终止时，进程共享资源（锁，条件变量，信号量，文件描述符）等没有被释放，当进程中的最后一个线程终止时，进程通过调用exit()终止，进程共享资源才会被释放
void pthread_exit(void *retval);
```



#### 连接线程

```c++
// \brief 等待thread指定的线程终止，如果retval不为NULL，则pthread_join()拷
// 贝目标线程的退出状态
int pthread_join(pthread_t thread, void **retval);
```

- 多个线程同时连接同一个线程的结果是未定义的；如果调用该函数的线程被取消，则目标线程仍然是可连接的。
- 两个线程互相连接或者线程连接自己将导致死锁。
- 同一个进程中的线程是对等的，任何线程可以连接同一个进程中的任何其他的线程



#### 取消线程

```c++
// \brief 向thread对应的线程发送一个取消请求，该线程是否被取消以及何时取消取决
// 于该线程的取消状态和取消类型
// 返回值仅代表取消请求是否成功排队
int pthread_cancel(pthread_t thread);

// \brief 设置线程的取消状态
// 取消状态可能的取值：PTHREAD_CANCEL_ENABLE（默认值） 或 
// PTHREAD_CANCEL_DISABLE
// 如果线程设置不可取消，则取消请求将会保留在队列中，知道取消状态设置为可取消；
// 如果线程设置为可取消，则线程何时取消取决于取消类型。
int pthread_setcancelstate(int state, int *oldstate);

// \brief 设置线程的取消类型
// 取消类型可能的取值：PTHREAD_CANCEL_DEFERRED（默认值） 或
// PTHREAD_CANCEL_ASYNCHRONOUS
// 异步取消：线程可以在任何时间取消（通常是立即取消，但是系统不保证这一点）。
// 延迟取消：线程取消请求将会被延迟到线程接下来调用的一个是取消点的函数。很少有用
// 到，因为这样线程就可以随时被取消，就不能安全的持有资源（如分配的内存、获取互斥量、信号量等）
int pthread_setcanceltype(int type, int *oldtype);
```

执行取消请求有以下三个步骤（也适用于pthread_exit()）：

1. 弹出取消清理函数（以被压入的相反顺序弹出），pthread_cleanup_push()
2. 以未指定的顺序析构线程特有数据
3. 线程终止

在一个被取消的线程终止之后，通过pthread_join()将获取到线程的退出状态为PTHREAD_CANCELED。连接一个线程是知道取消已经完成的唯一方法。



***

