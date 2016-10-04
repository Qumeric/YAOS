#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define TIMER_COMMAND_PORT 0x43
#define TIMER_ZERO_CHANNEL 0x40

void init_timer(uint8_t, uint16_t);

#endif /*__TIMER_H__*/
