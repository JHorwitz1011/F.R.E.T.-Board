#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MOSI 4
#define SCK  2
#define MISO 3

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

// Write 1 byte to the specified register
void dac8411_write( spi_inst_t *spi, 
                        uint8_t pwr,
                        uint16_t data) {
    //construct packet
    uint32_t pkt = ((pwr << DAC8411_PWR_SHIFT) & DAC8411_PWR_MASK) | ((data << DAC8411_DATA_SHIFT) & DAC8411_DATA_MASK);
    uint8_t b1 = ((pkt & 0xFF0000) >> 16);
    uint8_t b3 = (pkt & 0x0000FF) << 16;
    pkt = (0x00FF00 & pkt) | b1 | b3; 

    printf("spitting out %d\n", pkt);
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
    spi_init(spi, 1000 * 1000);
    dac8411_init(spi);

    // Loop forever
    float counter = 0;
    while (true) {
        counter += 0.01;
        printf("loop..\n");
        dac8411_write(spi, DAC8411_POWER_NORMAL, (int)(0x7FFF*sin(counter) + 0x7FFF));
    }
}