#pragma once

#include "hardware/spi.h"
#include "filters.h"

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

void dac8411_write(spi_inst_t *spi, uint8_t pwr, sfint data);
void dac8411_init(spi_inst_t* spi);