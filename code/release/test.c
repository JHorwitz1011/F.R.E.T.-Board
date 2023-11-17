#include <stdlib.h>
#include "filters.h"
#include "ad7685.h"
#include "dac8411.h"

int main() {
    Coeff* coeffs = malloc(sizeof(Coeff));
     
    memset(coeffs, 0x00, sizeof(Coeff));
    fillCoefficients(coeffs);

    // Ports
    spi_inst_t *spi = spi0;

    // Initialize chosen serial port
    stdio_init_all();
    // Initialize SPI port at 1 MHz
    spi_init(spi, 10 * 1000 * 1000);
    ad7685_init(spi);
    dac8411_init(spi);

    s7x16 inputs[3] = {0,0,0};
    s7x16 outputs[3] = {0,0,0};

    while (true) {
        gpio_put(18, 1);
        sleep_us(10);
        gpio_put(18, 0);
        inputs[2] = inputs[1];
        inputs[1] = inputs[0];
        inputs[0] = ad7685_read(spi);
        filter(&inputs, &outputs);
        dac8411_write(spi, DAC8411_POWER_NORMAL, outputs[0]);
    }

    free(coeffs);
}