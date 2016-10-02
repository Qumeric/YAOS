#include <stdint.h>
// Stdint?!
#include <serial.h>
#include <ioport.h>
#include <helpers.h>
// Why <> ?

void init_serial_port() {
    // Disable interruptions
    out8(SP_ADRESS + 1, 0);

    // Get ready to recieve a divisor
    out8(SP_ADRESS + 3, bit(7));

    // Set divisor
    out8(SP_ADRESS + 0, lsb0(SP_DIVISOR));
    out8(SP_ADRESS + 1, lsb1(SP_DIVISOR));

    // Set frame format
    uint8_t frame_format = 0;
    if (SP_FRAME_SIZE % 2 == 0)
        frame_format |= bit(0);
    if (SP_FRAME_SIZE >= 7)
        frame_format |= bit(1);

    if (SP_STOP_BITS > 1) // if 3rd bit is 0 then use only 1 stop bit
        frame_format |= bit(2);

    // FIXME Parity check?
    // FIXME bit of divisor access?

    out8(SP_ADRESS + 3, frame_format);

    write_string_to_stdout("Serial Port has been initialized!\n");
}

void write_byte_to_stdout(uint8_t byte) {
    while (!get_bit(in8(SP_ADRESS + 5), 5))
        ; // polling

    out8(SP_ADRESS+0, byte);
}

void write_string_to_stdout(const char *str) {
    for (size_t i = 0; str[i]; i++) {
        write_byte_to_stdout(str[i]);
    }
}
