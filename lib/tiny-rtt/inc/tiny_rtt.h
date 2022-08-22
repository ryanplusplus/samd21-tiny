/*!
 * @file
 * @brief
 */

#ifndef tiny_rtt_h
#define tiny_rtt_h

#include "SEGGER_RTT.h"

static inline void tiny_rtt_init(void)
{
  SEGGER_RTT_Init();
}

#define tiny_rtt_printf(...) \
  SEGGER_RTT_printf(0, __VA_ARGS__)

#endif
