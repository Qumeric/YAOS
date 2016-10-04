#include <timer.h>
#include <helpers.h>
#include <ioport.h>
#include <idt.h>
#include <pic.h>

void init_timer(uint8_t mode, uint16_t value) {
    send_master_data(0xff ^ bit(2)); // mask everything

    out8(TIMER_COMMAND_PORT, (mode << 1) | bit(4) | bit(5));
    out8(TIMER_ZERO_CHANNEL, BYTE0(value));
    out8(TIMER_ZERO_CHANNEL, BYTE1(value));

    send_master_data(0xfe ^ bit(2)); // unmask timer FIXME bad style?
}
