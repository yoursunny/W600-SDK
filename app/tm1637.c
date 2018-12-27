#include "tm1637.h"

#include <assert.h>
#include <string.h>

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

void
tm1637_setBlank(tm1637* tm)
{
  memset(tm->segments, 0, sizeof(tm->segments));
}

void
tm1637_addDot(tm1637* tm, int pos)
{
  if (pos < 0 || pos >= 4) {
    return;
  }
  tm->segments[pos] |= 0b10000000;
}

static const uint8_t DIGIT_SEGMENTS[] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
};

uint8_t
tm1637_digitToSegments(int digit)
{
  if (digit < 0 || digit > 9) {
    return 0;
  }
  return DIGIT_SEGMENTS[digit];
}

bool
tm1637_setNumber(tm1637* tm, int n, bool pad)
{
  if (n < 0 || n > 9999) {
    return false;
  }
  for (int i = 3; i >= 0; --i) {
    int digit = n % 10;
    tm->segments[i] = tm1637_digitToSegments(digit);
    if (!pad && n == 0 && i < 3) {
      tm->segments[i] = 0;
    }
    n /= 10;
  }
  return true;
}

bool
tm1637_setFloat(tm1637* tm, float n)
{
  if (n < 0.0 || n >= 9999.0) {
    return false;
  }

  if (n < 1.0) {
    tm1637_setNumber(tm, (int)(n * 1000), true);
    tm1637_addDot(tm, 0);
    return true;
  }

  int dot = 3;
  while (n * 10 < 9999.0) {
    n *= 10;
    --dot;
  }
  assert(dot >= 0);
  tm1637_setNumber(tm, n, true);
  tm1637_addDot(tm, dot);
  return true;
}
