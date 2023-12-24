/*!
 * @file
 * @brief
 */

#ifndef neopixel_pa11_h
#define neopixel_pa11_h

#include "clock.h"
#include "sam.h"

#define neopixel_api_name neopixel_pa11
#define neopixel_byte_order neopixel_byte_order_grb
#define neopixel_pin PIN_PA11
#define neopixel_clock_speed clock_gclk_main_frequency

#include "neopixel_h_template.h"

#endif
