#include "pico/stdlib.h"

#include "filters.h"
#include "ad7685.h"
#include "dac8411.h"
#include "pins.h"

int main() {
    Coeff* coeffs = initCoefficients();
    Gains* gains = initGains();

    // Ports
    spi_inst_t *spi = spi0;

    // Initialize chosen serial port
    stdio_init_all();
    // Initialize SPI port at 1 MHz
    spi_init(spi, 10 * 1000 * 1000);
    ad7685_init(spi);
    dac8411_init(spi);

    s7x16 inputs[3];
    s7x16 outputs[3];

    while (true) {
        gpio_put(UTILITY1, 1);
        sleep_us(1);
        gpio_put(UTILITY1, 0);
        inputs[2] = inputs[1];
        inputs[1] = inputs[0];
        inputs[0] = ad7685_read(spi);
        gpio_put(UTILITY2, 1);
        sleep_us(1);
        gpio_put(UTILITY2, 0);
        filter(inputs, outputs, coeffs, gains);
        gpio_put(UTILITY3, 1);
        sleep_us(1);
        gpio_put(UTILITY3, 0);
        dac8411_write(spi, DAC8411_POWER_NORMAL, outputs[0]);
    }

    deinitCoefficients(coeffs);
    deinitGains(gains);
}