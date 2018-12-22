#include "wm_include.h"

static const int LED_PINS[] = {
  WM_IO_PB_08,
  WM_IO_PB_09,
  WM_IO_PB_10,
  WM_IO_PB_11,
  WM_IO_PB_12,
};
static const int LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

void
UserMain()
{
  for (int i = 0; i < LED_COUNT; ++i) {
    tls_gpio_cfg(LED_PINS[i], WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  }

  while (true) {
    for (int i = 0; i < LED_COUNT; ++i) {
      for (int j = 0; j < LED_COUNT; ++j) {
        tls_gpio_write(LED_PINS[j], i == j);
      }
      tls_os_time_delay(HZ);
    }
  }
}
