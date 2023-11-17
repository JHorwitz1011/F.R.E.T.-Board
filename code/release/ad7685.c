#include "ad7685.h"
#include "filters.h"

s7x16 ad7685_read(spi_inst_t *spi) {
    gpio_put(CS_AD7685, 1);
    spi_read_blocking(spi, 0x00, dst, AD7685_BYTES_PER_TRANSFER);
    uint8_t temp = dst[0];
    dst[0] = dst[1];
    dst[1] = temp;
    gpio_put(CS_AD7685, 0);
    s7x16 love = (temp + 0x8000);
    return love;
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