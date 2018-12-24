#include "tm1637.h"

#define TM1637_BITDELAY() \
  do { \
    for (int i = 0; i < 0x03FF; ++i) { \
      *((volatile uint32_t*)0xE0000000); \
    } \
  } while(false)

#define TM1637_LOW(pin) \
  do { \
    tls_gpio_cfg((pin), WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); \
    tls_gpio_write((pin), 0); \
  } while(false)

#define TM1637_HIGH(pin) \
  do { \
    tls_gpio_cfg((pin), WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING); \
  } while(false)

static void
tm1637_start(tm1637* tm)
{
  TM1637_LOW(tm->dio);
  TM1637_BITDELAY();
}

static void
tm1637_stop(tm1637* tm)
{
  TM1637_LOW(tm->dio);
  TM1637_BITDELAY();
  TM1637_HIGH(tm->clk);
  TM1637_BITDELAY();
  TM1637_HIGH(tm->dio);
  TM1637_BITDELAY();
}

static bool
tm1637_writeByte(tm1637* tm, uint8_t b)
{
  for (int i = 0; i < 8; ++i) {
    TM1637_LOW(tm->clk);
    TM1637_BITDELAY();

    if ((b & 0x01) == 0) {
      TM1637_LOW(tm->dio);
    } else {
      TM1637_HIGH(tm->dio);
    }
    TM1637_BITDELAY();

    TM1637_HIGH(tm->clk);
    TM1637_BITDELAY();

    b >>= 1;
  }

  TM1637_LOW(tm->clk);
  TM1637_HIGH(tm->dio);
  TM1637_BITDELAY();

  TM1637_HIGH(tm->clk);
  TM1637_BITDELAY();

  uint8_t ack = tls_gpio_read(tm->dio);
  if (ack == 0) {
    TM1637_LOW(tm->dio);
  }
  TM1637_BITDELAY();

  TM1637_LOW(tm->clk);
  TM1637_BITDELAY();

  return ack;
}

void
tm1637_show(tm1637* tm)
{
  tm1637_start(tm);
  tm1637_writeByte(tm, 0x40);
  tm1637_stop(tm);

  tm1637_start(tm);
  tm1637_writeByte(tm, 0xC0);
  for (int i = 0; i < 4; ++i) {
    tm1637_writeByte(tm, tm->segments[i]);
  }
  tm1637_stop(tm);

  tm1637_start(tm);
  tm1637_writeByte(tm, 0x8F);
  tm1637_stop(tm);
}
