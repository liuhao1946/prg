#include "app_clk.h"


/*
移植说明：

1、根据自己的需求设置合适的定时任务数量，定时任务数量由CLK_TASK_MAX_NUM（定义在app_clk.h中）决定

2、如果要使每个定时任务尽量占用少的RAM，需要把a_size_t（app_clk.h）改为unsigned char
*/


app_clk_t sf_timer;
volatile  uint8_t timer_1ms_f;

//定时任务
void test_100ms(void )
{
    static uint8_t clk;

    if(++clk >= 10)
    {
        clk = 0;
        //运行1s后，该定时任务停止运行(test()函数不再执行)，如果之后某个时候
        //还要执行，调用app_clk_start(&sf_timer)即可
        app_clk_stop(&sf_timer);
    }
}

//硬件定时器的中断函数
void timer_interrupt()
{
    timer_1ms_f = 1;
}

//1ms硬件定时器初始化函数
void timer_init()
{

}

main()
{
    timer_init();
    //创建一个延迟10ms第一次运行之后每隔100ms运行一次的软件定时器
    //该定时器在创建后不立刻运行
    app_construct_clk（&sf_timer, test_100ms, 10, 100, 0);
    //启动定时任务
    app_clk_start(&sf_timer);
    while(1)
    {
        if(timer_1ms_f)
        {
            timer_1ms_f = 0;
            app_clk_scheduler();
        }
    }
}