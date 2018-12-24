#ifndef TM1637_H
#define TM1637_H

#include <wm_include.h>

typedef struct tm1637
{
  enum tls_io_name clk;
  enum tls_io_name dio;
  uint8_t segments[4];
} tm1637;

void tm1637_show(tm1637* tm);

#endif // TM1637_H
