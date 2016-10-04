#include <ioport.h>
#include <helpers.h>
#include <serial.h>
#include <stdint.h>

void init_serial_port() {
    // Disable interruptions
    out8(SP_ADRESS + 1, 0);

    // Get ready to recieve a divisor (set DLAB)
    out8(SP_ADRESS + 3, bit(7));

    // Set divisor
    out8(SP_ADRESS + 0, BYTE0(SP_DIVISOR));
    out8(SP_ADRESS + 1, BYTE1(SP_DIVISOR));

    // Set frame format
    uint8_t frame_format = 0;
    if (SP_FRAME_SIZE % 2 == 0)
        frame_format |= bit(0);
    if (SP_FRAME_SIZE >= 7)
        frame_format |= bit(1);

    if (SP_STOP_BITS > 1) // if 3rd bit is 0 then use only 1 stop bit
        frame_format |= bit(2);

    out8(SP_ADRESS + 3, frame_format);

    // FIXME enable IRQs, set RTS/DSR (http://wiki.osdev.org/Serial_Ports)

    sp_write_string("Serial Port has been initialized!\n");
}

void sp_write_byte(uint8_t byte) {
    while (!get_bit(in8(SP_ADRESS + 5), 5))
        ; // polling

    out8(SP_ADRESS+0, byte);
}

void sp_write_string(const char *str) {
    for (size_t i = 0; str[i]; i++) {
        sp_write_byte(str[i]);
    }
}
