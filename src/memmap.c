#include <memmap.h>
#include <multiboot.h>
#include <stdint.h>
#include <print.h>

extern const uint32_t multiboot_info;
extern const char text_phys_begin[];
extern const char bss_phys_end[];

void init_memmap() {
    typedef unsigned long long ull;
    multiboot_info_t *mb_info = (multiboot_info_t*) (ull) multiboot_info;

    enum { MMAP_IS_PRESENT_FLAG = 1 << 6 };

    if ((mb_info->flags & MMAP_IS_PRESENT_FLAG) == 0) {
        printf("Memory map is missing in multiboot info\n");
        return;
    }

    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) (ull) mb_info->mmap_addr;
    while((ull) mmap < mb_info->mmap_addr + mb_info->mmap_length) {
        multiboot_uint64_t first_byte = mmap->addr;
        multiboot_uint64_t last_byte  = first_byte + mmap->len;
        printf("memory range: 0x%llx-0x%llx type = 0x%x\n",
                first_byte, last_byte, (unsigned) mmap->type);
        mmap = (multiboot_memory_map_t*)((ull) mmap + mmap->size + sizeof(mmap->size));
    }


    printf("kernel memory range: 0x%llx-0x%llx\n",
            (ull) text_phys_begin, (ull) bss_phys_end);
}
