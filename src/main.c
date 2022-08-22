/*!
 * @file
 * @brief
 */

#include "board.h"
#include "tiny_timer.h"
#include "tiny_rtt.h"

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

  tiny_rtt_init();
  tiny_rtt_printf("Hello, RTT!\n");

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
