/*!
 * @file
 * @brief
 */

#include "sam.h"
#include "clock.h"
#include "uart_sercom3_pa22_pa23.h"
#include "tiny_event.h"

#ifdef SERCOM3

static tiny_event_t send_complete;
static tiny_event_t receive;

void SERCOM3_Handler(void)
{
  if(SERCOM3->USART.INTFLAG.bit.TXC) {
    SERCOM3->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    tiny_event_publish(&send_complete, NULL);
  }

  while(SERCOM3->USART.INTFLAG.bit.RXC) {
    const uint8_t byte = SERCOM3->USART.DATA.reg;
    tiny_event_publish(&receive, &(tiny_uart_on_receive_args_t){ .byte = byte });
  }
}

static void send(i_tiny_uart_t* self, uint8_t byte)
{
  (void)self;
  SERCOM3->USART.DATA.reg = byte;
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
  // Enable SERCOM3 clock
  PM->APBCMASK.bit.SERCOM3_ = 1;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM3_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[22 >> 1].bit.PMUXE = MUX_PA22C_SERCOM3_PAD0;
  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[23 >> 1].bit.PMUXO = MUX_PA23C_SERCOM3_PAD1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;

  SERCOM3->USART.CTRLA.reg =
    SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
    SERCOM_USART_CTRLA_TXPO(0) | // SERCOM3_PAD0 used for TX
    SERCOM_USART_CTRLA_RXPO(1) | // SERCOM3_PAD1 used for RX
    SERCOM_USART_CTRLA_DORD; // LSB first

  SERCOM3->USART.CTRLB.reg =
    SERCOM_USART_CTRLB_TXEN |
    SERCOM_USART_CTRLB_RXEN;
  while(SERCOM3->USART.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM3->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / clock_gclk0_frequency);

  SERCOM3->USART.INTENSET.reg =
    SERCOM_USART_INTENSET_TXC |
    SERCOM_USART_INTENSET_RXC;

  NVIC_EnableIRQ(SERCOM3_IRQn);

  SERCOM3->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM3->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart_sercom3_pa22_pa23_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);

  static i_tiny_uart_t self;
  self.api = &api;
  return &self;
}

#endif
