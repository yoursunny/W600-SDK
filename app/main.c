#include <wm_include.h>

static void
UserTask(void* taskParam)
{
  while (true) {
    tls_os_time_delay(1);
  }
}

void
UserMain()
{
  static OS_STK stack[2048];
  tls_os_task_create(NULL, NULL, UserTask, NULL, (u8*)stack, sizeof(stack) / sizeof(stack[0]), 32, 0);
}
