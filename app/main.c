#include "tm1637.h"
#include <wm_include.h>

static void
UserTask(void* taskParam)
{
  tm1637 tm;
  tm.clk = WM_IO_PB_11;
  tm.dio = WM_IO_PB_12;
  tm.segments[0] = 0b00111111;
  tm.segments[1] = 0b00000110;
  tm.segments[2] = 0b01011011;
  tm.segments[3] = 0b01001111;

  while (true) {
    tm1637_show(&tm);
    tls_os_time_delay(1);
  }
}

void
UserMain()
{
  static OS_STK stack[2048];
  tls_os_task_create(NULL, NULL, UserTask, NULL, (u8*)stack, sizeof(stack) / sizeof(stack[0]), 32, 0);
}
