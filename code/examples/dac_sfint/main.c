#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ad7685.h"
#include "util.h"
#include "dac8411.h"

float sampleToVoltage(uint16_t input) {
    float precision = 65536;
    float value = (float)input;
    float rail = 3.3;
    return rail*value/precision;
}

/*******************************************************************************
 * Main
 */
int main() {

    // Ports
    spi_inst_t *spi = spi0;

    // Initialize chosen serial port
    stdio_init_all();

    // Initialize SPI port at 1 MHz
    spi_init(spi, 10*1000 * 1000);
    ad7685_init(spi);
    dac8411_init(spi);

    // Loop forever
    float counter = 0;
    while (true) {
        counter += 0.01;
        printf("loop..\n");
        dac8411_write(spi, DAC8411_POWER_NORMAL, float_to_sfint(32767.0f * sin(counter)));
    }
}