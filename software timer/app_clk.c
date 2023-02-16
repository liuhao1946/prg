
#include "app_clk.h"

////////////////////////////////////////////////////////////////////////////////
//function declare

////////////////////////////////////////////////////////////////////////////////
//global define 
app_clk_t clk_task[CLK_TASK_MAX_NUM];

////////////////////////////////////////////////////////////////////////////////

static unsigned char app_search_none_cb(a_cb_t clk_cb)
{
   unsigned char index = 0;
    
    while(clk_task[index].clk_cb != 0 && index < CLK_TASK_MAX_NUM)
    {
      if(clk_task[index].clk_cb == clk_cb)
      {
        return CALLBACK_REPEAT;
      }
      
      index++;
    }
    
    return index;
}

static unsigned char app_search_cb(a_cb_t clk_cb)
{
    unsigned char index = 0;
    
    while(clk_task[index].clk_cb != clk_cb && index < CLK_TASK_MAX_NUM)
    {
      index++;
    }
    
    return index;
}

static void app_clk_del(unsigned char index)
{
  if(index < CLK_TASK_MAX_NUM)
  {
    clk_task[index].clk_cb = 0;
    clk_task[index].delay  = 0;
    clk_task[index].peroid = 0;
  }
}

void app_clk_stop(app_clk_t *clk)
{
  if(clk == 0) return;
    
  app_clk_del(app_search_cb(clk->clk_cb));
}

void app_clk_start(app_clk_t *clk)
{
  unsigned char index;  
  
  if(clk == 0) return;
  
  index = app_search_none_cb(clk->clk_cb);
  
  if(index < CLK_TASK_MAX_NUM)
  {
    clk_task[index].clk_cb = clk->clk_cb;
    clk_task[index].delay  = clk->delay;
    clk_task[index].peroid = clk->peroid;
  }
}

status_t app_construct_clk(app_clk_t *clk, a_cb_t clk_cb, a_size_t dly, a_size_t period, unsigned char run_flag)
{
  clk->clk_cb = clk_cb;
  clk->delay  = dly;
  clk->peroid = period;
  
  if(run_flag)
  {
    app_clk_start(clk);
  }
  
  return CONSTRUCT_SUC;
}

void app_clk_scheduler(void )
{
  unsigned char index = 0;
  
  for(index = 0; index < CLK_TASK_MAX_NUM; index++)
  {
    if(clk_task[index].clk_cb != 0)
    {
      if(clk_task[index].delay != 0)
      {
         clk_task[index].delay--; 
      }
      else
      {
         clk_task[index].clk_cb();
         clk_task[index].delay = clk_task[index].peroid;
         
         if(clk_task[index].peroid == 0)
         {
           app_clk_del(index);
         }
      }
    }
  }
  
}





















