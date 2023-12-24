/*!
 * @file
 * @brief
 */

#ifndef board_h
#define board_h

#include "async_spi_sercom4_pa12_pb10_pb11.h"
#include "buffered_uart_sercom0_pa10_pa11.h"
#include "clock.h"
#include "heartbeat_pa17.h"
#include "i2c_sercom3_pa22_pa23.h"
#include "interrupts.h"
#include "neopixel_pa14.h"
#include "sam.h"
#include "spi_sercom4_pa12_pb10_pb11.h"
#include "systick.h"
#include "uart_sercom0_pa10_pa11.h"
#include "watchdog.h"

#define heartbeat_init heartbeat_pa17_init
#define uart_init uart_sercom0_pa10_pa11_init
#define buffered_uart_init buffered_uart_sercom0_pa10_pa11_init
#define spi_init spi_sercom4_pa12_pb10_pb11_init
#define async_spi_init async_spi_sercom4_pa12_pb10_pb11_init
#define i2c_init i2c_sercom3_pa22_pa23_init
#define neopixel_bff neopixel_pa14
#define neopixel_bff_color_t neopixel_concat(neopixel_bff, _color_t)
#define neopixel_bff_init neopixel_concat(neopixel_bff, _init)
#define neopixel_bff_reset neopixel_concat(neopixel_bff, _reset)
#define neopixel_bff_write neopixel_concat(neopixel_bff, _write)
#define neopixel_bff_write_all neopixel_concat(neopixel_bff, _write_all)

#endif
