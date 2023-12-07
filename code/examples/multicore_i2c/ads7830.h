#pragma once

#include "hardware/i2c.h"

#include "filters.h"
//i2c address bits
#define ADS7830_ADDR_READ  0b10010001
#define ADS7830_ADDR_WRITE 0b10010000

// 1001000 00100101 0000

//power bits
#define ADS7830_PD_BITS_OFFSET 2
#define ADS7830_PD_MASK 0b00001100
#define ADS7830_PD_IREF_OFF_AD_BT_CONV_OFF 0b00
#define ADS7830_PD_IREF_OFF_AD_BT_CONV_ON 0b01
#define ADS7830_PD_IREF_ON_AD_BT_CONV_OFF 0b10
#define ADS7830_PD_IREF_ON_AD_BT_CONV_ON 0b11

//port configuration bits - single ended / diff ended
#define ADS7830_PORT_BITS_OFFSET 7
#define ADS7830_PORT_MASK 0b10000000
#define ADS7830_CMD_SE 1
#define ADS7830_CMD_DE 0

//channel bits
#define ADS7830_CH_BITS_OFFSET 4
#define ADS7830_CH_MASK 0b01110000
#define ADS7830_CH0 0b000
#define ADS7830_CH1 0b100
#define ADS7830_CH2 0b001
#define ADS7830_CH3 0b101
#define ADS7830_CH4 0b010

#define ADS7830_CH5 0b110
#define ADS7830_CH6 0b011
#define ADS7830_CH7 0b111

void ads7830_init(i2c_inst_t* i2c);
void ads7830_write(i2c_inst_t* i2c, uint8_t ch, uint8_t pwr_mode);
uint8_t ads7830_read(i2c_inst_t* i2c, uint8_t ch, uint8_t pwr_mode);
void updateGains(i2c_inst_t* i2c, Gains* g);