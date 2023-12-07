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

#define SYS_FS 44100
#define SYS_PERIOD_US 1000000/SYS_FS

float sampleToVoltage(uint16_t input) {
    float precision = 65536;
    float value = (float)input;
    float rail = 3.3;
    return rail*value/precision;
}
//23 + 
spi_inst_t* spi;
i2c_inst_t* i2c;

Coeff* coeffs;
Gains* gains;
Feedbacks* fb;

sfint inputs[3];
sfint output;
repeating_timer_t timer;

bool timer_callback(repeating_timer_t *rt);

bool timer_callback(repeating_timer_t *rt) {
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
    return true;
}

/*******************************************************************************
 * Main
 */
int main() {
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
    spi_init(spi, 20*1000 * 1000);  //10Mhz
    i2c_init(i2c, 400 * 1000);      //400khz
    ad7685_init(spi);
    dac8411_init(spi);
    ads7830_init(i2c);
    
    //buffer vars
    inputs[0] = 0;
    inputs[1] = 0;
    inputs[2] = 0;
    output = 0;


    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_us(-SYS_PERIOD_US, timer_callback, NULL, &timer)) {
        printf("Failed to add timer\n");
        return 1;
    }

    while(1) {
        sleep_ms(10000);
    }
    cancel_repeating_timer(&timer);
    deinitGains(gains);
    deinitFeedbacks(fb);
    deinitCoefficients(coeffs);
}//7.3 + 11.34 + 4.3