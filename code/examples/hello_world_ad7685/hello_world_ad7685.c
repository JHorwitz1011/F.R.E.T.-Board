#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MOSI 4
#define SCK  2
#define MISO 3

#define CS_AD7685 5
#define CPOL_AD7685 0
#define CPHA_AD7685 0

#define AD7685_BYTES_PER_TRANSFER       2
#define AD7685_BITS_PER_TRANSFER        8   

/*******************************************************************************
 * Function Definitions
 */

// Write 1 byte to the specified register
void ad7685_read(spi_inst_t *spi, uint8_t* dst) {
    gpio_put(CS_AD7685, 1);
    spi_read_blocking(spi, 0x00, dst, AD7685_BYTES_PER_TRANSFER);
    uint8_t temp = dst[0];
    dst[0] = dst[1];
    dst[1] = temp;
    gpio_put(CS_AD7685, 0);
}

float sampleToVoltage(uint16_t input) {
    float precision = 65536;
    float value = (float)input;
    float rail = 3.3;
    return rail*value/precision;
}

void ad7685_init(spi_inst_t* spi) {
    // Initialize CS pin high
    gpio_init(CS_AD7685);
    gpio_set_dir(CS_AD7685, GPIO_OUT);
    gpio_put(CS_AD7685, 0);

    // Set SPI format
    spi_set_format( spi,                            // SPI instance
                    AD7685_BITS_PER_TRANSFER,       // Number of bits per transfer
                    CPOL_AD7685,                    // Polarity (CPOL)
                    CPHA_AD7685,                    // Phase (CPHA)
                    SPI_MSB_FIRST);

    // Initialize SPI pins
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
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
    spi_init(spi, 1000 * 1000);
    ad7685_init(spi);

    // Loop forever
    while (true) {
        // printf("loop..\n");
        uint16_t sample = 0;
        ad7685_read(spi, (uint8_t*)&sample); 
        printf("%f, %d\n", sampleToVoltage(sample), sample);
        sleep_ms(5000);
    }
}