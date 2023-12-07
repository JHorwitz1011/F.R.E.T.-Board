#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ad7685.h"
#include "util.h"


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
    spi_init(spi, 15*1000 * 1000);
    ad7685_init(spi);

    // Loop forever
    while (true) {
        // printf("loop..\n");
        sfint sample = ad7685_read(spi); 
        printf("uint16_t %d, sfint %f, voltage %f\n", sfint_to_uint16(sample), sfint_to_float(sample), sampleToVoltage(sfint_to_uint16(sample)));
        // printBits(4, &sample);
        // printf("\n");
        sleep_ms(10);
    }
}