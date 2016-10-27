#include <memmap.h>
#include <multiboot.h>
#include <stdint.h>
#include <print.h>

extern const uint32_t multiboot_info;
extern const char text_phys_begin[];
extern const char bss_phys_end[];

void init_memmap() {
    multiboot_info_t *mb_info = (multiboot_info_t*) multiboot_info;

    // FIXME flags?
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mb_info->mmap_addr;
    while((multiboot_uint32_t) mmap < mb_info->mmap_addr + mb_info->mmap_length) {
        multiboot_uint64_t first_byte = mmap->addr;
        multiboot_uint64_t last_byte  = first_byte + mmap->len;
        printf("memory range: 0x%x%x-0x%x%x type = 0x%x\n",
                first_byte >> 32, first_byte & 0xffffffff,
                last_byte >> 32,  last_byte  & 0xffffffff,
                mmap->type);
        mmap = (multiboot_memory_map_t*)(mmap + mmap->size + sizeof(mmap->size));
    }


    printf("kernel memory range: 0x%x%x-0x%x%x\n",
            (uint64_t) text_phys_begin >> 32, (uint32_t) text_phys_begin & 0xffffffff,
            (uint64_t) bss_phys_end >> 32,  (uint32_t) bss_phys_end  & 0xffffffff);
}
