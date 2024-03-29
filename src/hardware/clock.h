/*!
 * @file
 * @brief
 */

#ifndef clock_h
#define clock_h

#define clock_gclk0_frequency 48000000
#define clock_gclk2_frequency 32768
#define clock_gclk_main_frequency clock_gclk0_frequency

void clock_init(void);

#endif
