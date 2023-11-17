#include "dac8411.h"


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