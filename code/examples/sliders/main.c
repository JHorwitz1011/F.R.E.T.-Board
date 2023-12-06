#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "ad7685.h"
#include "util.h"
#include "dac8411.h"
#include "ads7830.h"

float sampleToVoltage(uint16_t input) {
    float precision = 65536;
    float value = (float)input;
    float rail = 3.3;
    return rail*value/precision;
}

spi_inst_t* spi;
i2c_inst_t* i2c;


/*******************************************************************************
 * Main
 */
int main() {
    Coeff* coeffs;
    Gains* gains;
    Feedbacks* fb;

    stdio_init_all();

    // Ports
    spi = spi0;
    i2c = i2c0;

    // Initialize chosen serial port
    stdio_init_all();

    // Initialize SPI port at 1 MHz
    spi_init(spi, 10*1000 * 1000);
    i2c_init(i2c, 400 * 1000); // 400khz

    
    ad7685_init(spi);
    dac8411_init(spi);
    ads7830_init(i2c);
//1001 000 0010 0001 0000
//210
    while(1) {

        printf("\n\n\nBEGIN!\n");
        printf("S1: %d\n", ads7830_read(i2c, ADS7830_CH0, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON));
        printf("S2: %d\n", ads7830_read(i2c, ADS7830_CH1, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON));
        printf("S3: %d\n", ads7830_read(i2c, ADS7830_CH2, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON));
        printf("S4: %d\n", ads7830_read(i2c, ADS7830_CH3, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON));
        printf("S5: %d\n", ads7830_read(i2c, ADS7830_CH4, ADS7830_PD_IREF_OFF_AD_BT_CONV_ON));
         
        sleep_ms(1000);
    }
}