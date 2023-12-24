/*!
 * @file
 * @brief
 */

#include "interrupts.h"
#include "sam.h"

#ifndef neopixel_pin
#error "neopixel_pin must be defined"
#endif

#ifndef neopixel_clock_speed
#error "neopixel_clock_speed must be defined"
#endif

static void send_byte(uint8_t byte)
{
#if neopixel_clock_speed == 48000000UL
  __asm__ __volatile__(
    "_send_byte:\n"
    "  mov r4, #8\n"
    "\n"
    "send_bit:\n"
    "  str %[_mask], [%[_outset]]\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  lsl %[byte], %[byte], #1\n"
    "  tst %[byte], %[_carry]\n"
    "  bne send_1\n"
    "\n"
    "send_0:\n"
    "  str %[_mask], [%[_outclr]]\n"
    "\n"
    "	 mov r5, #8\n"
    "send_0_loop:\n"
    "	 sub r5, r5, #1\n"
    "  nop\n"
    "	 bne send_0_loop\n"
    "\n"
    "  nop\n"
    "  nop\n"
    "  sub r4, r4, #1\n"
    "  bne send_bit\n"
    "  b done\n"
    "\n"
    "send_1:\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  sub r4, r4, #1\n"
    "  str %[_mask], [%[_outclr]]\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  nop\n"
    "  bne send_bit\n"
    "\n"
    "done:\n"
    :
    : [byte] "r"(byte), [_outset] "l"(&PORT->Group[neopixel_pin / 32].OUTSET.reg), [_outclr] "l"(&PORT->Group[neopixel_pin / 32].OUTCLR.reg), [_mask] "r"(1 << (neopixel_pin % 32)), [_carry] "r"(0x100)
    : "r4", "r5", "memory");
#else
#error "Unsupported neopixel_clock_speed"
#endif
}

void neopixel_concat(neopixel_api_name, _init)(void)
{
  PORT->Group[neopixel_pin / 32].PINCFG[neopixel_pin % 32].bit.PMUXEN = 0;
  PORT->Group[neopixel_pin / 32].DIRSET.reg = 1 << (neopixel_pin % 32);
}

void neopixel_concat(neopixel_api_name, _write)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count)
{
  interrupts_critical_section({
    for(uint16_t i = 0; i < count; i++) {
      for(uint8_t j = 0; j < sizeof(data[0]); j++) {
        send_byte(((const uint8_t*)data)[i * sizeof(data[0]) + j]);
      }
    }
  });
}

void neopixel_concat(neopixel_api_name, _write_all)(const neopixel_concat(neopixel_api_name, _color_t) data, uint16_t count)
{
  interrupts_critical_section({
    for(uint16_t i = 0; i < count; i++) {
      for(uint8_t j = 0; j < sizeof(data); j++) {
        send_byte(((const uint8_t*)&data)[j]);
      }
    }
  });
}

void neopixel_concat(neopixel_api_name, _reset)(void)
{
  // 50 usec is required for reset

#if neopixel_clock_speed == 48000000UL
  // 50 usec == 2400 cycles @ 48 MHz
  __asm__ __volatile__(
    "reset_neopixel:\n"
    // 0x350 == ~850
    "	 mov r4, #0x35\n"
    "	 lsl r4, #4\n"
    "loop:\n"
    "	 sub r4, r4, #1\n"
    "	 bne loop\n"
    :
    :
    : "r4");
#else
#error "Unsupported neopixel_clock_speed"
#endif
}
