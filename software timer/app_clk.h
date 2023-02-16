

#ifndef _APP_CLK_
#define _APP_CLK_


#define CLK_TASK_MAX_NUM  10  //定时任务最大数量



typedef unsigned short int a_size_t;  
typedef void (* a_cb_t)(void ); 

typedef struct
{
   a_cb_t   clk_cb;
   a_size_t delay;  
   a_size_t peroid;
}app_clk_t;

typedef enum
{
  CONSTRUCT_FAULT,
  CONSTRUCT_SUC,
  
  CALLBACK_REPEAT = (unsigned char) 0xff
}status_t;

/*******************************************************************************
**函数功能: 创建一个软件系统时钟
**输入参数: clk       - 用户定义的时钟实例
**
**          clk_cb    - 要定期执行的回调函数,该函数由用户定义
**
**          dly       - 创建时钟后,延迟多久执行相应的clk_cb。具体dly延时多久取决
**                      app_clk_scheduler()的定时间隔。假如app_clk_scheduler()的
**                      定时间隔1ms，dly = 100，则clk_cb延迟100ms后执行   
**                  
**          peroid    - 间隔period后再次运行clk_cb。当peroid = 0, 表示clk_cb只执行
**                      一次,当peroid != 0,clk_cb循坏执行
**
**          run_flag  - 如果创建完成后立刻运行则   - TRUE
**                      如果创建完成后不要立刻运行 - FALSE
**输出参数: status_t   - 如果创建成功  - CONSTRUCT_SUC
                      - 如果创建失败  - CONSTRUCT_FAULT
*******************************************************************************/
status_t app_construct_clk(app_clk_t *clk, a_cb_t clk_cb, a_size_t dly, a_size_t period, unsigned char run_flag);

/*******************************************************************************
**函数功能: 软件系统时钟调度,该函数调用间隔决定了用户的定时函数最少间隔多久
**          执行一次
**
**输入参数: 空
**输出参数: 空
*******************************************************************************/
void app_clk_scheduler(void );

/*******************************************************************************
**函数功能: 启动定时函数
**
**输入参数: clk - 用户定义的时钟实例
**
**输出参数: 空
*******************************************************************************/
void app_clk_start(app_clk_t *clk);

/*******************************************************************************
**函数功能: 停止定时函数。要注意的是，当定时任务停止后，该任务对应存储空间会释放。比如
**         CLK_TASK_MAX_NUM = 10，当前已经有了10个定时任务，当调用app_clk_stop(&a)
**         删除一个定时任务后，当前还能再加入一个定时任务。当调用app_clk_start(&a)，
**         该定时任务又再次占用定时器内存空间，此后就不能再加入其他定时任务
**
**输入参数: clk - 用户定义的时钟实例
**
**输出参数:
*******************************************************************************/
void app_clk_stop(app_clk_t *clk);

/*

例子：
app_clk_t timer;

//定时任务
void test_100ms(void )
{

}

1、放置调度器函数app_clk_scheduler()放到定时任务中周期调用，假设每隔1ms调用一次

2、构建软件定时任务，该任务10ms后启动，之后每隔100ms运行一次
app_construct_clk（&timer, test_100ms, 10, 100, 1);

3、停止定时器
app_clk_stop(&timer);

4、启动定时器
app_clk_start(&timer);

*/



#endif