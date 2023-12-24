/*!
 * @file
 * @brief
 */

#include "board.h"
#include "tiny_rtt.h"
#include "tiny_timer.h"
#include "tiny_utils.h"

static tiny_timer_group_t timer_group;

int main(void)
{
  interrupts_disable();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, systick_init());
    watchdog_init(&timer_group);
    heartbeat_init(&timer_group);
  }
  interrupts_enable();

  neopixel_bff_color_t colors[] = {
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
  };
  neopixel_bff_init();
  neopixel_bff_reset();
  neopixel_bff_write(colors, element_count(colors));

  tiny_rtt_init();
  tiny_rtt_printf("Hello, RTT!\n");

  while(1) {
    neopixel_bff_write(colors, element_count(colors));

    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
