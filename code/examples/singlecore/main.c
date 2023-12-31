#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "ad7685.h"
#include "util.h"
#include "dac8411.h"
#include "ads7830.h"
#include "pins.h"

float sampleToVoltage(uint16_t input) {
    float precision = 65536;
    float value = (float)input;
    float rail = 3.3;
    return rail*value/precision;
}
//23 + 
spi_inst_t* spi;
i2c_inst_t* i2c;


/*******************************************************************************
 * Main
 */
int main() {
    Coeff* coeffs;
    Gains* gains;
    Feedbacks* fb;

    //initialize filter parameters
    coeffs = initCoefficients();
    gains = initGains();
    fb = initFeedbacks();

    //some debug gpios
    gpio_init(UTIL1);
    gpio_set_dir(UTIL1, GPIO_OUT);
    gpio_put(UTIL1, 1);
    
    // Initialize chosen serial port
    stdio_init_all();

    // init peripherals
    spi = spi0;
    i2c = i2c0;
    spi_init(spi, 20*1000 * 1000);  //20Mhz
    i2c_init(i2c, 400 * 1000);      //400khz
    ad7685_init(spi);
    dac8411_init(spi);
    ads7830_init(i2c);
    
    //buffer vars
    sfint inputs[3] = {0};
    sfint output = 0;

    while(1) {
        step(inputs);
        stepFb(fb);
        inputs[2] = ad7685_read(spi);
        // printf("%f\n", sfint_to_float(inputs[2]));
        output = filter(inputs+2, fb, coeffs, gains);
        // output = inputs[2];
        gpio_put(UTIL1, 0);
        dac8411_write(spi, DAC8411_POWER_NORMAL, output);
        gpio_put(UTIL1, 1);
        // sleep_ms(1);
    }
    deinitGains(gains);
    deinitFeedbacks(fb);
    deinitCoefficients(coeffs);
}//7.3 + 11.34 + 4.3