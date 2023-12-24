/*!
 * @file
 * @brief
 */

#include "clock.h"
#include "sam.h"
#include "tiny_event.h"
#include "uart_sercom2_pa08_pa09.h"

#ifdef SERCOM2

static tiny_event_t send_complete;
static tiny_event_t receive;

void SERCOM2_Handler(void)
{
  if(SERCOM2->USART.INTFLAG.bit.TXC) {
    SERCOM2->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    tiny_event_publish(&send_complete, NULL);
  }

  while(SERCOM2->USART.INTFLAG.bit.RXC) {
    const uint8_t byte = SERCOM2->USART.DATA.reg;
    tiny_event_publish(&receive, &(tiny_uart_on_receive_args_t){ .byte = byte });
  }
}

static void send(i_tiny_uart_t* self, uint8_t byte)
{
  (void)self;
  SERCOM2->USART.DATA.reg = byte;
}

static i_tiny_event_t* on_send_complete(i_tiny_uart_t* self)
{
  (void)self;
  return &send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_uart_t* self)
{
  (void)self;
  return &receive.interface;
}

static inline void initialize_peripheral(uint32_t baud)
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

  PORT->Group[0].PMUX[8 >> 1].bit.PMUXE = MUX_PA08D_SERCOM2_PAD0;
  PORT->Group[0].PINCFG[8].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[9 >> 1].bit.PMUXO = MUX_PA09D_SERCOM2_PAD1;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;

  SERCOM2->USART.CTRLA.reg =
    SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
    SERCOM_USART_CTRLA_TXPO(0) | // SERCOM2_PAD0 used for TX
    SERCOM_USART_CTRLA_RXPO(1) | // SERCOM2_PAD1 used for RX
    SERCOM_USART_CTRLA_DORD; // LSB first

  SERCOM2->USART.CTRLB.reg =
    SERCOM_USART_CTRLB_TXEN |
    SERCOM_USART_CTRLB_RXEN;
  while(SERCOM2->USART.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM2->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / clock_gclk0_frequency);

  SERCOM2->USART.INTENSET.reg =
    SERCOM_USART_INTENSET_TXC |
    SERCOM_USART_INTENSET_RXC;

  NVIC_EnableIRQ(SERCOM2_IRQn);

  SERCOM2->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM2->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart_sercom2_pa08_pa09_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);

  static i_tiny_uart_t self;
  self.api = &api;
  return &self;
}

#endif
