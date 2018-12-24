#include "tm1637.h"
#include <wm_include.h>

static void
UserTask(void* taskParam)
{
  tm1637 tm;
  tm.clk = WM_IO_PB_11;
  tm.dio = WM_IO_PB_12;

  while (true) {
    for (int n = 0; n <= 9999; ++n) {
      tm1637_setNumber(&tm, n, true);
      tm1637_show(&tm);
      tls_os_time_delay(50);
    }
  }
}

void
UserMain()
{
  static OS_STK stack[2048];
  tls_os_task_create(NULL, NULL, UserTask, NULL, (u8*)stack, sizeof(stack) / sizeof(stack[0]), 32, 0);
}
