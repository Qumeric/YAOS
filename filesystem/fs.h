#ifndef __FS_H__
#define __FS_H__

#include <stdint.h>
#include <malloc.h>
#include <pthread.h>
#include <stdbool.h>

// a better alternative to #define
enum {
    MEM_SIZE    = 1 << 16,
    MAX_FILES   = 1 << 8,
    BLOCK_SIZE  = 1 << 12,
    MAX_NAME    = 64,
    MAX_DEPTH   = 32
};

typedef enum { READONLY = 1, WRITEONLY = 2, READWRITE = 4} flags_t;

typedef struct descriptor_s {
    pthread_mutex_t *lock;
    bool used;
    uint16_t flags;
    char* pathname;
} descriptor_t;

typedef struct block_s {
    pthread_mutex_t *lock;
    char memory[BLOCK_SIZE];
    size_t last;
} block_t;

typedef enum { NOTHING_T, FILE_T, DIR_T } entry_t;

typedef struct entry_info_s {
    pthread_mutex_t *lock;
    entry_t type;
    size_t blocks[256]; // FIXME much memory
    size_t last;
} entry_info_t;

typedef struct node_s {
    pthread_mutex_t *lock;
    char letter;
    uint16_t depth;
    struct node_s* parent;
    struct node_s* nodes[256];
    entry_info_t* info;
    struct node_s** dir_entries;
    size_t number_of_entries;
    size_t entries_capacity;
} node_t;

entry_info_t* add(const char*, node_t*);

int32_t open(const char *pathname, uint16_t flags);
int16_t close(int32_t fd);
size_t read(int32_t fd, char *buf, size_t count);
size_t write(int32_t fd, const char *buf, size_t count);
int16_t mkdir(const char *pathname);
char *readdir(char*);

int32_t get_empty_block();
int32_t get_empty_descriptor();

void init_filesystem();

#endif /*__FS_H__*/
