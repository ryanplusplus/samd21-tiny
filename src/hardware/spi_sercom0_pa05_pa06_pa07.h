/*!
 * @file
 * @brief
 */

#ifndef spi_sercom0_pa05_pa06_pa07_h
#define spi_sercom0_pa05_pa06_pa07_h

#include <stdbool.h>
#include <stdint.h>
#include "hal/i_tiny_spi.h"

i_tiny_spi_t* spi_sercom0_pa05_pa06_pa07_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
