#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

#define DESCRIPTORS_COUNT 64
#define INTERRUPT_GATE 14
#define TRAP_GATE 15

struct Descriptor_s {
    uint16_t offset1;
    uint16_t segment_selector;
    uint16_t flags;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t padding;
} __attribute__((packed));

typedef struct Descriptor_s Descriptor;

void init_idt();

void handle_timer();

#endif /*__IDT_H__*/
