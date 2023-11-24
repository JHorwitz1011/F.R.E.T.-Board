#include <stdio.h>
// #include <stdfix.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"

#include "filters.h"
#include "ad7685.h"
#include "dac8411.h"
#include "pins.h"

int main() {
    Coeff* coeffs = initCoefficients();
    Gains* gains = initGains();

    // Ports
    spi_inst_t *spi = spi0;
    i2c_inst_t *i2c = i2c0;

    i2c_init(i2c, 400 * 1000);

    gpio_init(UTIL2);
    gpio_set_dir(UTIL2, GPIO_OUT);
    

    // Initialize chosen serial port
    stdio_init_all();
    // Initialize SPI port at 1 MHz
    spi_init(spi, 10 * 1000 * 1000);
    ad7685_init(spi);
    dac8411_init(spi);

    s7x16 inputs[3];
    s7x16 outputs[3];

    while (true) {
        inputs[2] = inputs[1];
        inputs[1] = inputs[0];
        inputs[0] = ad7685_read(spi);

        // printf("%d\n", inputs[0]);
        filter(inputs, outputs, coeffs, gains);
        // sleep_us(2);
        dac8411_write(spi, DAC8411_POWER_NORMAL, outputs[0]);

        // gpio_put(UTIL2, 1);
        // sleep_us(1);
        // gpio_put(UTIL2, 0);
    }

    deinitCoefficients(coeffs);
    deinitGains(gains);
}