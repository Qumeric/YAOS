#ifndef __DESC_H__
#define __DESC_H__

#include <stdint.h>

struct desc_table_ptr_s {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

typedef struct desc_table_ptr_s DescriptorTablePtr;

static inline void read_idtr(DescriptorTablePtr *ptr)
{
	__asm__ ("sidt %0" : "=m"(*ptr));
}

static inline void write_idtr(const DescriptorTablePtr *ptr)
{
	__asm__ ("lidt %0" : : "m"(*ptr));
}

static inline void read_gdtr(DescriptorTablePtr *ptr)
{
	__asm__ ("sgdt %0" : "=m"(*ptr));
}

static inline void write_gdtr(const DescriptorTablePtr *ptr)
{
	__asm__ ("lgdt %0" : : "m"(*ptr));
}

#endif /*__DESC_H__*/
