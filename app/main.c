#include <wm_include.h>
#include <wm_gpio_afsel.h>
#include "htu21d.h"

void
UserMain()
{
  tls_gpio_cfg(WM_IO_PB_10, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_write(WM_IO_PB_10, true);
  tls_os_time_delay(HZ);

  wm_i2c_scl_config(WM_IO_PB_11);
  wm_i2c_sda_config(WM_IO_PB_12);
  tls_i2c_init(100000);

  if (!htu21d_reset()) {
    printf("htu21d_reset failed.\n");
    return;
  }

  while (true) {
    float temperature, humidity;
    if (htu21d_readTemperature(&temperature) && htu21d_readHumidity(&humidity)) {
      printf("%0.2f %0.2f\n", temperature, humidity);
    }
    tls_os_time_delay(HZ);
  }
}
