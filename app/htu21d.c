#include "htu21d.h"
#include <wm_i2c.h>

enum {
  HTU21D_I2CADDR  = 0x40,
  HTU21D_READTEMP = 0xE3,
  HTU21D_READHUM  = 0xE5,
  HTU21D_WRITEREG = 0xE6,
  HTU21D_READREG  = 0xE7,
  HTU21D_RESET    = 0xFE,
};

#define SLEEP(millis) tls_os_time_delay(millis * HZ / 1000)

#define I2C_WRITE2(data, start) \
  do { \
    tls_i2c_write_byte(data, start); \
    if (tls_i2c_wait_ack() != WM_SUCCESS) { \
      return false; \
    } \
  } while(false)

#define I2C_START(addr, isRead) I2C_WRITE2((addr << 1) | (int)isRead, true)

#define I2C_WRITE(data) I2C_WRITE2(data, false)

#define I2C_READ(wantNack) tls_i2c_read_byte(!wantNack, false)

#define I2C_STOP() tls_i2c_stop()

bool
htu21d_reset()
{
  I2C_START(HTU21D_I2CADDR, false);
  I2C_WRITE(HTU21D_RESET);
  I2C_STOP();
  SLEEP(15);

  I2C_START(HTU21D_I2CADDR, false);
  I2C_WRITE(HTU21D_READREG);
  I2C_STOP();

  I2C_START(HTU21D_I2CADDR, true);
  uint8_t reg = I2C_READ(true);
  I2C_STOP();
  printf("reg=%02x\n", reg);

  return reg == 0x02;
}

bool
htu21d_readTemperature(float* result)
{
  I2C_START(HTU21D_I2CADDR, false);
  I2C_WRITE(HTU21D_READTEMP);
  I2C_STOP();
  SLEEP(50);

  I2C_START(HTU21D_I2CADDR, true);
  uint16_t t = I2C_READ(false);
  t <<= 8;
  t |= I2C_READ(false) & 0b11111100;
  I2C_READ(true); // crc
  I2C_STOP();

  *result = t;
  *result *= 175.72f;
  *result /= 65536.0f;
  *result -= 46.85f;
  return true;
}

bool
htu21d_readHumidity(float* result)
{
  I2C_START(HTU21D_I2CADDR, false);
  I2C_WRITE(HTU21D_READHUM);
  I2C_STOP();
  SLEEP(50);

  I2C_START(HTU21D_I2CADDR, true);
  uint16_t t = I2C_READ(false);
  t <<= 8;
  t |= I2C_READ(false) & 0b11111100;
  I2C_READ(true); // crc
  I2C_STOP();

  *result = t;
  *result *= 125.0f;
  *result /= 65536.0f;
  *result -= 6.0f;
  return true;
}
