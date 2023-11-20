#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pins.h"
#include "ad7685.h"

s7x16 ad7685_read(spi_inst_t *spi) {
    uint16_t dst = 0;
    uint8_t* dst_as_array = (uint8_t*)(&dst);
    gpio_put(CS_AD7685, 1);
    sleep_us(4);
    spi_read_blocking(spi, 0x00, dst_as_array, AD7685_BYTES_PER_TRANSFER);
    
    //flip byte order
    uint8_t temp = dst_as_array[0];
    dst_as_array[0] = dst_as_array[1]; 
    dst_as_array[1] = temp;
    gpio_put(CS_AD7685, 0);

    return (s7x16)(dst + 0x8000);
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