#include "htu21d.h"
#include "tm1637.h"
#include <wm_include.h>

static void
MeasureTask(void* taskParam)
{
  // supply power to HTU21D
  tls_gpio_cfg(WM_IO_PB_08, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_write(WM_IO_PB_08, true);
  tls_os_time_delay(HZ);

  // initialize HTU21D
  wm_i2c_scl_config(WM_IO_PB_11);
  wm_i2c_sda_config(WM_IO_PB_12);
  tls_i2c_init(100000);
  if (!htu21d_reset()) {
    printf("htu21d_reset failed.\n");
    return;
  }

  // perform measurement
  while (true) {
    float temperature, humidity;
    if (htu21d_readTemperature(&temperature) && htu21d_readHumidity(&humidity)) {
      printf("%0.2f %0.2f\n", temperature, humidity);
    }
    tls_os_time_delay(HZ);
  }
}

static void
DisplayTask(void* taskParam)
{
  // initialize display
  tm1637 tm;
  tm.clk = WM_IO_PB_09;
  tm.dio = WM_IO_PB_10;

  // update display
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
  static OS_STK measureStack[2048];
  tls_os_task_create(NULL, NULL, MeasureTask, NULL, (u8*)measureStack, sizeof(measureStack) / sizeof(measureStack[0]), 32, 0);
  static OS_STK displayStack[2048];
  tls_os_task_create(NULL, NULL, DisplayTask, NULL, (u8*)displayStack, sizeof(displayStack) / sizeof(displayStack[0]), 33, 0);
}
