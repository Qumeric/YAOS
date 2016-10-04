#include <desc.h>
#include <idt.h>
#include <helpers.h>
#include <memory.h>
#include <serial.h>
#include <pic.h>

Descriptor idt[DESCRIPTORS_COUNT];

extern uint64_t interruption_handlers[];

void add_descriptor(uint8_t position, // Position in IDT
                    uint64_t offset, // Address of interruption handler
                    uint16_t segment_selector, // CS register before handler call
                    uint8_t DPL, // Descriptor Priveledge Level
                    uint8_t TYPE // Trap Gate (15) or Interrupt Gate (14)
                    ) {
    // FIXME IST (don't use one stack)
    Descriptor *descriptor = idt + position;

    // FIXME little bit of magic
    descriptor->offset1 = offset & 0xffff;
    descriptor->segment_selector = segment_selector;
    descriptor->offset2 = (offset >> 16) & 0xffff;
    descriptor->offset3 = (offset >> 32) & 0xffffffff;
    descriptor->padding = 0;


    uint16_t p = bit(15); // 15th bit is set if the descriptor is valid
    // DPL is two bits. Lower value (0-3) means higher priveledge
    descriptor->flags = p | (DPL << 13) | (TYPE << 8);
}

void init_idt() {
    for (size_t i = 0; i < DESCRIPTORS_COUNT; i++) {
        add_descriptor(i,
                        interruption_handlers[i],
                        KERNEL_CS,
                        0,
                        INTERRUPT_GATE);
    }

    DescriptorTablePtr ptr = {sizeof(idt) - 1, (uint64_t)(idt)};
    write_idtr(&ptr);

    sp_write_string("IDT has been initialized!\n");
}

void disable_interruptions() {
    __asm__ volatile("cli":::"cc");
}

void enable_interruptions() {
    __asm__ volatile("sti":::"cc");
}

// FIXME unproper place?
void handle_interruption(uint64_t id) {
    handle_timer(); // FIXME
    switch(id) {
        case 0 :
            sp_write_string("Zero division\n");
            break;
            break;
        default :
            sp_write_string("Some interruption\n");
            break;
    }
}

void handle_timer() {
    sp_write_string("tick\n");
    eoi_master(); // FIXME unflexible
}
