#pragma once


#include "filters.h"
#include "hardware/spi.h"

#define CPOL_AD7685 0
#define CPHA_AD7685 0

#define AD7685_BYTES_PER_TRANSFER       2
#define AD7685_BITS_PER_TRANSFER        8

sfint ad7685_read(spi_inst_t* spi);
void ad7685_init(spi_inst_t* spi);