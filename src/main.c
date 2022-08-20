/*!
 * @file
 * @brief
 */

#include "board.h"
#include "tiny_timer.h"
#include "SEGGER_RTT.h"

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

  SEGGER_RTT_Init();
  SEGGER_RTT_printf(0, "derp");

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
