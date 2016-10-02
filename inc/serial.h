#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SP_ADRESS 0x3f8
#define SP_FRAME_SIZE 8
#define SP_STOP_BITS 1
#define SP_DIVISOR 1

void init_serial_port();
void write_byte_to_stdout(uint8_t);
void write_string_to_stdout(const char*);

#endif /*__SERIAL_H*/
