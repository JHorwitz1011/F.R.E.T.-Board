#include <stdio.h>

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

    sfint inputs[3];
    sfint outputs[3];

    while (true) {

        //shift buffers
        inputs[0] = inputs[1];
        inputs[1] = inputs[2];

        outputs[0] = outputs[1];
        outputs[1] = outputs[2];

        inputs[2] = ad7685_read(spi);

        // printf("%d\n", inputs[0]);
        outputs[2] = filter(inputs, outputs, coeffs, gains);
        // sleep_us(2);
        dac8411_write(spi, DAC8411_POWER_NORMAL, outputs[2]);

    }

    deinitCoefficients(coeffs);
    deinitGains(gains);
}