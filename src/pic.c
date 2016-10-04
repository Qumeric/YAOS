#include <ioport.h>
#include <helpers.h>
#include <pic.h>
#include <serial.h>

void send_master_command(uint8_t c) {
    out8(0x20, c);
}

void send_master_data(uint8_t d) {
    out8(0x21, d);
}

void send_slave_command(uint8_t c) {
    out8(0xA0, c);
}

void send_slave_data(uint8_t d) {
    out8(0xA1, d);
}

void init_pic() {

    // Initialize and wait for 3 bytes of data.
    // Also using cascade (bit 1) and edge triggered mode (bit 3).
    send_master_command(bit(4) | bit(0));
    send_master_data(IDT_MASTER_DESCRIPTOR1);
    send_master_data(bit(2)); // 3rd pin for slave (IBM PC cascade)
    send_master_data(1); // FIXME magic
    send_master_data(0xff ^ bit(2)); // Do not mask slave's pin

    sp_write_string("Master PIC has been initialized\n");

    send_slave_command(bit(4) | bit(0));
    send_master_data(IDT_SLAVE_DESCRIPTOR1);
    send_slave_data(2); // 3rd pin and not a bitmask
    send_slave_data(1);
    send_slave_data(0xff);
    __asm__("sti"); // enable CPU interruptions

    sp_write_string("Slave PIC has been initialized\n");
}

void eoi_master() {
    send_master_command(bit(5));
}

void eoi_slave() {
    send_slave_command(bit(5));
}
