#pragma once

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pins.h"

#define CPOL_AD7685 0
#define CPHA_AD7685 0

#define AD7685_BYTES_PER_TRANSFER       2
#define AD7685_BITS_PER_TRANSFER        8