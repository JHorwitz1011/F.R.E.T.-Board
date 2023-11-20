//c libraries
#include <stdio.h>
#include <math.h>

//pico libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"

//project files
#include "dac8411.h"
#include "pins.h"
#include "filters.h"

// Write 1 byte to the specified register
void dac8411_write( spi_inst_t *spi, 
                        uint8_t pwr,
                        s7x16 data) {
    //convert packet to unsigned type
    uint16_t cnv_data = data + 0x8000;
    //construct packet
    // uint32_t pkt = (pwr << DAC8411_PWR_SHIFT) | (cnv_data << DAC8411_DATA_SHIFT);
    // uint8_t b1 = ((pkt & 0xFF0000) >> 16);
    // uint8_t b3 = (pkt & 0x0000FF) << 16;
    // pkt = (0x00FF00 & pkt) | b1 | b3; 

    uint32_t pkt = (pwr << 6) | (cnv_data >> 10) | ((cnv_data  & 0b0000001111111100 ) << 6) | (0b11 << 22);

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