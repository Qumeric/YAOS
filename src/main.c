static void qemu_gdb_hang(void) {
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <ints.h>
#include <idt.h>
#include <pic.h>
#include <serial.h>
#include <timer.h>

void main(void) {
    qemu_gdb_hang();

    init_serial_port();
    init_idt();
    init_pic();

    __asm__("int $0");

    init_timer(2, 0xffffu);

    for(;;);
}
