#ifndef TM1637_H
#define TM1637_H

#include <wm_include.h>

/** \brief Config and state of TM1637 device.
 */
typedef struct tm1637
{
  enum tls_io_name clk;
  enum tls_io_name dio;
  uint8_t segments[4];
} tm1637;

/** \brief Output segments to TM1637 device.
 */
void tm1637_show(tm1637* tm);

/** \brief Encode decimal digit to segments.
 */
uint8_t tm1637_digitToSegments(int digit);

/** \brief Set segments to a non-negative integer.
 */
bool tm1637_setNumber(tm1637* tm, int n, bool pad);

#endif // TM1637_H
