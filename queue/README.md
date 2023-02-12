# app_queue模块的应用（一）

## 前言
这是一个FIFO通用队列模块，它不需要动态内存分配，具有较好的灵活性，对使用环境几乎没有要求。要特别注意的是，如果该模块应用在多线程环境下，要注意给相关接口增加保护。


## 模块API

* app_queue_init() - 初始化队列。

* app_enqueue() - 元素入队。

* app_dequeue() - 元素出队。

* app_queue_is_none() - 判断队列是否为空。

* app_queue_search() - 搜索队列中的指定元素。

* app_queue_clr() - 清除队列。

* app_queue_get_ele_num() - 获得队列中已有的元素个数。

* app_queue_get_remain_ele_num() - 获得队列中剩余空间能保存元素的个数。

* app_queue_traversal() - 遍历队列中的元素。

* app_queue_modify_speci_element() - 修改队列中指定元素的数据。

## 快速使用
* 定义队列
    ```c
    #define APP_QUEUE_MAX      10

    //要放入队列的元素
    typedef {
        uint16_t s1;
        uint8_t s2;
    }ele_t;

    //定义队列句柄
    typedef struct {
        qcb_t qcb;
        ele_t buf[APP_QUEUE_MAX];
    }q_handle_t;

    q_handle_t q_handle;
    ```
    >Note: 
    >1. **任何队列句柄结构体的第一个位置必须定义为 qcb_t qcb**。
    >2. 队列元素的结构体可根据用户需要自由定义。
    
* 初始化队列
    * 队列满后运行覆盖旧数据
        ```c
        app_queue_init(&q_handle, Q_BUF_LEN, sizeof(ele_t), NO_BLOCK);
        ```
    * 队列满后阻塞
        ```c
        app_queue_init(&q_handle, Q_BUF_LEN, sizeof(ele_t), BLOCK);
        ```

    如果应用在**多线程**环境，需要对该接口予以保护（互斥量或者全局中断），比如：
    ```c
    //进入中断保护
    app_queue_init();
    //退出中断保护
    ```
    
* 数据入队
    ```c
    ele_t ele;
  
    ele.s1 = 60;
    ele.s2 = 6000;
    //多线程环境下，需要进入中断保护
    app_enqueue(&q_handle, &ele);
    //多线程环境下，退出中断保护
    ```

* 数据出队
    ```c
    ele_t ele;

    //多线程环境下，需要进入中断保护
    app_dequeue(&q_handle, &ele);
    //多线程环境下，退出中断保护
    ```

* 判断队列是否为空
    ```c
    //多线程环境下，需要进入中断保护
    if (app_queue_is_none() != Q_NONE) {
  		app_dequeue(&q_handle, &ele);
    }
    //多线程环境下，退出中断保护
    ```

* 搜索指定元素是否在队列中。有时候你可能不想队列中有重复的元素。
    ```c
    //要搜索的元素
    ele_t ele; 
    
    ele.s1 = 0x01;
    ele.s2 = 0x02;
    //多线程环境下，需要进入中断保护
    if (app_queue_search(&q_handle, &ele) == Q_NONE) {
        
    }
    //多线程环境下，退出中断保护
    ```

* 遍历与修改队列。有时候你可能需要遍历队列中是否有目标元素，如果有目标元素，删除它。要注意的是，使用这里的接口并非真的把目标元素从队列中删除，而是通过修改目标元素的数据，在出队时，应用自己过滤掉这个元素。
    ```c
    //多线程环境下，需要进入中断保护
    //获得队列中元素的数量
    len = app_queue_get_ele_num(&q_handle); 
    for (i = 0 ; i < len; i++) {
        ele_t ele;

        app_queue_traversal(&q_handle, i, &ele);
        //找到目标元素
        if (ele.s1 == 0x01) {
            ele_t new_ele;

            new_ele.s1 == 0xff;
            //修改目标元素
            app_queue_modify_speci_element(&q_handle, i, &new_ele);
        }
    }
    //多线程环境下，退出中断保护

    //应用对队列的处理
    void app_queue_handler(void )
    {
        //多线程环境下，需要进入中断保护
        if (app_queue_is_none(&q_handle) == Q_NONE) {
             //多线程环境下，退出中断保护
            return;
        }
        //多线程环境下，退出中断保护

        ele_t ele;
        
        //多线程环境下，需要进入中断保护
        app_dequeue(&q_handle, &ele);
        //多线程环境下，退出中断保护

        switch(ele.s1) {
            case 0x01:
                //应用逻辑
                break;

            case 0x02:
                //应用逻辑
                break;
            
            default:
                //当队列元素的字段s1被修改为0xff时，执行这段代码从而过滤掉这个元素内容。
                break;
        }
    }
    ```

* 清除队列。
    ```c
    //多线程环境下，需要进入中断保护
    app_queue_clr(&q_handle);
    //多线程环境下，退出中断保护
    ```

## 进阶使用
有时候我们需要异步采样数据，比如一个心率处理程序除了需要它自身的PPG数据外，还需要外部加速度才能测量人在动态时的心率值。假设心率处理程序每隔320ms运行一次，每次运行时，它需要8个点的加速度值，这个时候我们可以使用这个队列完成这个功能要求。

* 定义以及初始化队列
    ```c
    //队列缓冲区设置为8的2倍以应对极端情况
    #define ACC_BUF_LEN     16

    typedef struct {
        int16_t x;
        int16_t y;
        int16_t z;
    }axis_t;

    typedef struct {
        qcb_t qcb;
        axis_t acc[ACC_BUF_LEN];
    }q_handle_t;
    
    q_handle_t q_handle;
    
    app_queue_init(&q_handle, ACC_BUF_LEN, sizeof(axis_t), NO_BLOCK);
    ```
    >Note：队列必须定义为非阻塞，这是为了确保总能从队列中拿到最新的数据。

* 加速度每隔40ms（25Hz）入队一次
    ```c
    axis_t a;

    a.x = -1;
    a.y = -2;
    a.z = -3;
    //保护进入
    app_enqueue(&q_handle, &a);
    //保护退出
    ```

* 320ms读取一次队列
    ```c
    #define READ_ACC_LEN        8

    axis_t acc[READ_ACC_LEN]; 

    for (i = 0; i < READ_ACC_LEN; i++) {
         //保护进入
        if (app_queue_traverse(&q_handle, i, &acc[i]) == Q_ERROR) {
            //如果要读取的长度 < 队列的实际长度时数组补0
            acc[i].x = 0;
            acc[i].y = 0;
            acc[i].z = 0;
        }
        //保护退出
    }

    //将加速度值输入到心率接口处理
    ```
    >Note: 这里遍历接口不能使用app_dequeue()，因为该接口会将值弹出队列，而使用app_queue_traverse()则不会。这是为了防止在320ms到来后，加速度队列可能不足8组数据，这时由于队列中过往的数据没有弹出，总能得到8组合适的数据满足心率需要。

## 版本
V2.0.0 Completed by bds on 2023.2.12

* 初始版本

[下一页](https://gitee.com/bds123/prg/blob/master/queue/README1.md)