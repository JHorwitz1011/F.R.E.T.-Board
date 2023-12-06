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

#define CS_DAC8411 6
#define CPOL_DAC8411 1
#define CPHA_DAC8411 0

#define BITS_PER_TRANSFER_DAC8411       24
#define DAC8411_PWR_MASK                0b110000000000000000000000
#define DAC8411_PWR_SHIFT               22
#define DAC8411_DATA_MASK               0b001111111111111111000000
#define DAC8411_DATA_SHIFT              6

#define DAC8411_POWER_NORMAL            0b00
#define DAC8411_POWER_1k_GND            0b01
#define DAC8411_POWER_100k_GND          0b10
#define DAC8411_POWER_HI_Z              0b11


/*******************************************************************************
 * Function Definitions
 */

void ad7685_read(spi_inst_t *spi, uint8_t* dst) {
    gpio_put(CS_AD7685, 1);
    sleep_us(4); //sleep to allow 7685 to sample adc value
    spi_read_blocking(spi, 0x00, dst, AD7685_BYTES_PER_TRANSFER);
    uint8_t temp = dst[0];
    dst[0] = dst[1];
    dst[1] = temp;
    gpio_put(CS_AD7685, 0);
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
 * Function Definitions
 */

// Write 1 byte to the specified register
void dac8411_write( spi_inst_t *spi, 
                        uint8_t pwr,
                        uint16_t data) {
    //construct packet
    uint32_t pkt = (pwr << 6) | (data >> 10) | ((data  & 0b0000001111111100 ) << 6) | ((data & 0b11) << 22);

    //send over spi
    gpio_put(CS_DAC8411, 0);
    spi_write_blocking(spi, (uint8_t*)&pkt, 3);
    gpio_put(CS_DAC8411, 1);
}

void dac8411_init(spi_inst_t* spi) {
    // Initialize CS pin high
    gpio_init(CS_DAC8411);
    gpio_set_dir(CS_DAC8411, GPIO_OUT);
    gpio_put(CS_DAC8411, 1);
    // Set SPI format
    spi_set_format( spi,   // SPI instance
                    BITS_PER_TRANSFER_DAC8411,      // Number of bits per transfer
                    CPOL_DAC8411,      // Polarity (CPOL)
                    CPHA_DAC8411,      // Phase (CPHA)
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
    spi_init(spi, 10 * 1000 * 1000);
    ad7685_init(spi);
    dac8411_init(spi);

    // Loop forever
    float counter = 0;
        uint16_t sample = 0;
    while (true) {
        ad7685_read(spi, (uint8_t*)&sample);
        // printf("%d\n", sample);
        // sleep_ms(10);
        dac8411_write(spi, DAC8411_POWER_NORMAL, sample);

    }
}
