#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

#define SP_ADRESS 0x3f8
#define SP_FRAME_SIZE 8
#define SP_STOP_BITS 1
#define SP_DIVISOR 1

void init_serial_port();
void sp_write_byte(uint8_t);
void sp_write_string(const char*);

#endif /*__SERIAL_H*/
