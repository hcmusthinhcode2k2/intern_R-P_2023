#ifndef LCD_I2C_CFG_H
#define LCD_I2C_CFG_H

#include "drivers/i2c_simple_master.h"
#include "../stick/SystemTick.h"

#define LCD_I2C_writeNByte(slv, pdata, length)   i2c_writeNBytes(slv, pdata, length)
#define LCD_I2C_readNByte(slv, pdata, length)    i2c_readNBytes(slv, pdata, length)

#define LCD_I2C_Address                          0x27

#define  __delay_us(us)                         __delay_ms(1)

#endif