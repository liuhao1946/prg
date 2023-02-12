# app_queue模块的应用（二）

这是一个FIFO通用队列模块，与前一个队列（app_queue_xxx）不同的是，这个队列专门用来处理批量数据。比如处理log，因为log的内容全部是字符型数据，采用前一个队列其处理效率不高，因此针对这种应用，设计了一个更为高效的队列接口。

## 模块API

* app_batch_queue_init() - 初始化队列。

* app_batch_data_enqueue() - 按照指定长度入队。

* app_batch_data_dequeue() - 按照指定长度出队。

* app_batch_queue_remain_ele_num() - 队列的剩余空间能存储的元素个数。

* app_batch_queue_get_ele_num() - 队列中的元素个数。

* app_batch_queue_clr() - 清除队列的所有元素。

* app_batch_queue_is_none() - 判断队列是否为空。


## 快速使用

* 初始化队列
    ```c
    //队列长度
    #define BQ_BUF_LEN		1024
    
    //用于队列缓冲区
    uint8_t buf[BQ_BUF_LEN];
  
    batch_q_t bq;

    //函数的第四个参数指的是队列中一个元素大小为1字节，如果一个元素大小为2字节，则该参数为2，如果大小为4，该参数为4
    //多线程环境下，需要进入中断保护
    app_batch_queue_init(&bq, buf, BQ_BUF_LEN, 1);
    //多线程环境下，退出中断保护
    ```

    >Note: 为了提高处理效率，该队列的**缓冲区长度**必须定义为**2的倍数**。
* 数据入队
    ```c
    char log[100];

    //多线程环境下，需要进入中断保护
    app_batch_data_enqueue(&bp, log, 100);
    //多线程环境下，退出中断保护
    ```

* 数据出队
    ```c
    char log[512];
    
    //多线程环境下，需要进入中断保护
    //从队列中读取240个字节数据，如果缓冲区内不足240字节，则全部读出到log中。
    app_batch_data_dequeue(&bq, 240, log, sizeof(log));
    //多线程环境下，退出中断保护
    ```

    有些写log数据到sd卡的应用中，为了提高写sd卡的效率，要求512字节的整数倍写入，此时可以这么做。
    ```c
     char log[2048];

    //多线程环境下，需要进入中断保护
    //确保每次从队列读取512的倍数字节数
    if (app_batch_queue_get_ele_num(&bq) >= 2048) {
        app_batch_data_dequeue(&bq, 2048, log, sizeof(log));
    }
    //多线程环境下，退出中断保护
    ```
    >Note: 就这个应用来说，必须要确保log缓冲区的长度要≥2048。

* 清除队列的所有元素
    ```c
    //多线程环境下，需要进入中断保护
    app_batch_queue_clr(&bq);
    //多线程环境下，退出中断保护
    ```

## 版本
V2.0.0 Completed by bds on 2023.2.12

* 初始版本