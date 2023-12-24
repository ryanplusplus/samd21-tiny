/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include "dma.h"
#include "interrupts.h"
#include "sam.h"
#include "tiny_utils.h"

enum {
  dma_channel_count = 12
};

static DmacDescriptor descriptor[dma_channel_count] __attribute__((__aligned__(16)));
static DmacDescriptor write_back_descriptor[dma_channel_count] __attribute__((__aligned__(16)));

typedef struct {
  void* context;
  dma_interrupt_callback_t callback;
} dma_interrupt_handler_t;

static dma_interrupt_handler_t handler[dma_channel_count];

static uint8_t next_channel;
static uint16_t enabled_channels;

static void init(void)
{
  static bool initialized;

  if(initialized) {
    return;
  }

  initialized = true;

  // Enable DMAC clock
  PM->AHBMASK.bit.DMAC_ = 1;
  PM->APBBMASK.bit.DMAC_ = 1;

  DMAC->BASEADDR.reg = (uintptr_t)descriptor;
  DMAC->WRBADDR.reg = (uintptr_t)write_back_descriptor;

  // Enable all priority levels
  DMAC->CTRL.bit.LVLEN0 = 1;
  DMAC->CTRL.bit.LVLEN1 = 1;
  DMAC->CTRL.bit.LVLEN2 = 1;
  DMAC->CTRL.bit.LVLEN3 = 1;

  NVIC_EnableIRQ(DMAC_IRQn);
}

uint8_t dma_channel_claim(void)
{
  init();

  if(next_channel >= dma_channel_count) {
    NVIC_SystemReset();
  }

  return next_channel++;
}

DmacDescriptor* dma_channel_descriptor(uint8_t channel)
{
  return &descriptor[channel];
}

DmacDescriptor* dma_channel_write_back_descriptor(uint8_t channel)
{
  return &write_back_descriptor[channel];
}

void dma_channel_enable(uint8_t channel, uint8_t trigger_action, uint8_t trigger_source, uint8_t priority)
{
  interrupts_critical_section({
    DMAC->CHID.bit.ID = channel;
    DMAC->CHCTRLB.bit.TRIGACT = trigger_action;
    DMAC->CHCTRLB.bit.TRIGSRC = trigger_source;
    DMAC->CHCTRLB.bit.LVL = priority;
    DMAC->CHCTRLA.bit.ENABLE = 1;
  });

  enabled_channels |= (1 << channel);

  DMAC->CTRL.bit.DMAENABLE = 1;
}

void dma_channel_disable(uint8_t channel)
{
  interrupts_critical_section({
    DMAC->CHID.bit.ID = channel;
    DMAC->CHCTRLA.bit.ENABLE = 0;
  });

  enabled_channels &= ~(1 << channel);

  if(!enabled_channels) {
    DMAC->CTRL.bit.DMAENABLE = 0;
  }
}

void dma_channel_trigger(uint8_t channel)
{
  DMAC->SWTRIGCTRL.vec.SWTRIG |= (1 << channel);
}

void dma_channel_enable_interrupt(uint8_t channel)
{
  interrupts_critical_section({
    DMAC->CHID.bit.ID = channel;
    DMAC->CHINTENSET.bit.TCMPL = 1;
  });
}

void dma_channel_disable_interrupt(uint8_t channel)
{
  interrupts_critical_section({
    DMAC->CHID.bit.ID = channel;
    DMAC->CHINTENSET.bit.TCMPL = 0;
  });
}

void dma_channel_install_interrupt_handler(uint8_t channel, void* context, dma_interrupt_callback_t callback)
{
  handler[channel].context = context;
  handler[channel].callback = callback;
}

void DMAC_Handler(void)
{
  for(uint8_t channel = 0; channel < next_channel; channel++) {
    bool transfer_complete = false;

    interrupts_critical_section({
      DMAC->CHID.bit.ID = channel;

      if(DMAC->CHINTFLAG.bit.TCMPL) {
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL;
        transfer_complete = true;
      }
    });

    if(transfer_complete && handler[channel].callback) {
      handler[channel].callback(handler[channel].context);
    }
  }
}
