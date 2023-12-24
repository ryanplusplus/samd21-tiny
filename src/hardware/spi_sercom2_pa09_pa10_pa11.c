/*!
 * @file
 * @brief
 */

#include "clock.h"
#include "sam.h"
#include "spi_sercom2_pa09_pa10_pa11.h"
#include "tiny_utils.h"

#ifdef SERCOM2

static void transfer(
  i_tiny_spi_t* self,
  const void* _write_buffer,
  void* _read_buffer,
  uint16_t buffer_size)
{
  (void)self;

  reinterpret(write_buffer, _write_buffer, const uint8_t*);
  reinterpret(read_buffer, _read_buffer, uint8_t*);

  for(uint32_t i = 0; i < buffer_size; i++) {
    while(!SERCOM2->SPI.INTFLAG.bit.DRE) {
    }

    SERCOM2->SPI.DATA.reg = write_buffer[i];

    while(!SERCOM2->SPI.INTFLAG.bit.RXC) {
    }

    if(read_buffer) {
      read_buffer[i] = SERCOM2->SPI.DATA.reg;
    }
    else {
      uint8_t dummy_read = SERCOM2->SPI.DATA.reg;
      (void)dummy_read;
    }
  }

  while(!SERCOM2->SPI.INTFLAG.bit.TXC) {
  }
}

static inline void initialize_peripheral(bool cpol, bool cpha, bool msb_first, uint32_t baud)
{
  // Enable SERCOM2 clock
  PM->APBCMASK.bit.SERCOM2_ = 1;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM2_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[9 >> 1].bit.PMUXO = MUX_PA09D_SERCOM2_PAD1;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[10 >> 1].bit.PMUXE = MUX_PA10D_SERCOM2_PAD2;
  PORT->Group[0].PINCFG[10].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[11 >> 1].bit.PMUXO = MUX_PA11D_SERCOM2_PAD3;
  PORT->Group[0].PINCFG[11].bit.PMUXEN = 1;

  SERCOM2->SPI.CTRLA.reg =
    SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
    SERCOM_SPI_CTRLA_DOPO(1) | // SERCOM2_PAD2, 3 used for MOSI, SCK
    SERCOM_SPI_CTRLA_DIPO(1) | // SERCOM2_PAD1 used for MISO
    (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) |
    (cpol << SERCOM_SPI_CTRLA_CPOL_Pos) |
    (msb_first ? 0 : SERCOM_SPI_CTRLA_DORD);

  SERCOM2->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
  while(SERCOM2->SPI.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM2->SPI.BAUD.reg = (clock_gclk0_frequency - 2 * baud) / (2 * baud);

  SERCOM2->SPI.CTRLA.bit.ENABLE = 1;
  while(SERCOM2->SPI.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_spi_api_t api = { transfer };

i_tiny_spi_t* spi_sercom2_pa09_pa10_pa11_init(bool cpol, bool cpha, bool msb_first, uint32_t baud)
{
  initialize_peripheral(cpol, cpha, msb_first, baud);

  static i_tiny_spi_t self;
  self.api = &api;
  return &self;
}

#endif
