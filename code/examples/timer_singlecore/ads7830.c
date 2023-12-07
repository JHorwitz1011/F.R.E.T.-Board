#include "pins.h"
#include "ads7830.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "filters.h"
#include "util.h"

#include <stdio.h>
void ads7830_write(i2c_inst_t* i2c, uint8_t ch, uint8_t pwr_mode) {
    uint8_t msg = (ADS7830_CMD_SE << ADS7830_PORT_BITS_OFFSET) | (ch << ADS7830_CH_BITS_OFFSET) | (pwr_mode << ADS7830_PD_BITS_OFFSET);
    i2c_write_blocking(i2c, 0x48, &msg, 1, false);
    printBits(1, &msg);
    printf("\n");
}

uint8_t ads7830_read(i2c_inst_t* i2c, uint8_t ch, uint8_t pwr_mode) {
    uint32_t dst = 0;
    ads7830_write(i2c, ch, pwr_mode);
    i2c_read_blocking(i2c, 0x48, (uint8_t*)&dst, 1, true);
    // printf("dst %d\n", dst);
    return dst;
}

void ads7830_init(i2c_inst_t* i2c) {
    // Initialize I2C pins
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C); 

    // set default configuration / channel to 0
    ads7830_write(i2c, ADS7830_CH0, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON);
}


//converts i2c reading into valid db range
sfint convert(uint8_t input) {

}

void update_gains(Gains* g) {
    
}