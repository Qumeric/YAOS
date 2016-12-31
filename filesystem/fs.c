#include <stdint.h>
#include <string.h>
#include "fs.h"


block_t *memory[MEM_SIZE];
descriptor_t *descriptors[MAX_FILES];
node_t *trie;

descriptor_t *new_descriptor() {
    descriptor_t *d = calloc(1, sizeof *d);
    d->lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(d->lock, NULL);
    return d;
}

node_t *new_node(entry_t type, char letter) {
    //printf("Creating node\n");
    node_t *node = calloc(1, sizeof(node_t));
    node->lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(node->lock, NULL);
    node->info = calloc(1, sizeof(entry_info_t));
    node->info->lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(node->info->lock, NULL);
    node->info->type = type;
    if (type == DIR_T) {
        node->entries_capacity = 1;
        node->dir_entries = calloc(node->entries_capacity, sizeof(node_t**));
        node->number_of_entries = 0;
    }
    node->letter = letter;
    return node;
}

block_t* new_block() {
    block_t *b = calloc(1, sizeof(block_t));
    b->lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(b->lock, NULL);
    return b;
}

void add_trie_entry(node_t* dir, node_t* entry) {
    if (dir->entries_capacity == dir->number_of_entries) {
        dir->entries_capacity *= 2;
        dir->dir_entries = realloc(dir->dir_entries, dir->entries_capacity * sizeof(node_t**));
    }
    dir->dir_entries[dir->number_of_entries] = entry;
    dir->number_of_entries++;
}

// Add path to given trie. etype=NOTHING for intermediate nodes
// pathname cannot be empty. if pathname ends with "/" then directory will be created
entry_info_t *add(const char *pathname, node_t *trie) {
    size_t ptr = 0;
    node_t* last_slash = trie;
    bool is_new = false;
    while (pathname[ptr] != '\0') {
        //printf("add step to %c\n", pathname[ptr]);
        pthread_mutex_t *oldLock = trie->lock;
        pthread_mutex_lock(oldLock);
        char l = pathname[ptr];
        if (trie->nodes[l] == NULL) {
            if (l == '/') {
                trie->nodes[l] = new_node(DIR_T, l);
            } else {
                trie->nodes[l] = new_node(NOTHING_T, l);
            }
            trie->nodes[l]->parent = trie;
            trie->nodes[l]->depth = (uint16_t) (trie->depth + 1);
            is_new = true;
        } else {
            is_new = false;
        }
        trie = trie->nodes[l];
        if (l == '/')
            last_slash = trie;
        pthread_mutex_unlock(oldLock);
        ptr++;
    }
    if (trie->info->type == NOTHING_T) {
        trie->info->type = FILE_T;
    }
    if (is_new) {
        add_trie_entry(last_slash, trie);
    }
    return trie->info;
}

// Caution: locks!
int32_t get_empty_block() {
    for (int32_t i = 0; i < MEM_SIZE; i++) {
        if (pthread_mutex_trylock(memory[i]->lock) == 0) {
            if (memory[i]->last == 0) {
                return i;
            }
            pthread_mutex_unlock(memory[i]->lock);
        }
    }
    return -1;
}

// Caution: locks!
int32_t get_empty_descriptor() {
    for (int32_t i = 0; i < MAX_FILES; i++) {
        if (descriptors[i] == NULL) {
            descriptors[i] = new_descriptor();
            return i;
        }
    }
    return -1;
}

// filename should not end with "/"
int32_t open(const char *pathname, uint16_t flags) {
    int32_t descriptor = get_empty_descriptor();
    if (descriptor == -1) {
        printf("No available file descriptors");
        return -1;
    }
    size_t len = strlen(pathname);
    descriptors[descriptor]->pathname = malloc(len+1);
    memcpy(descriptors[descriptor]->pathname, pathname, len);
    descriptors[descriptor]->pathname[len] = '\0';
    if (flags != READONLY) {
        pthread_mutex_lock(add(pathname, trie)->lock);
    }
    descriptors[descriptor]->flags = flags;
    descriptors[descriptor]->used = true;
    return descriptor;
}

// dirname should end with "/"
int16_t mkdir(const char *pathname) {
    add(pathname, trie);
    return 0; // indicates success
}

// Caution: don't forget to free
char* get_path(node_t* node) {
    char *buf = malloc(node->depth+1);
    buf[node->depth] = '\0';
    for (int ptr = node->depth - 1; ptr >= 0; ptr--) {
        printf("Node depth %d while ptr is %d  \n", node->depth, ptr);
        buf[ptr] = node->letter;
        node = node->parent;
    }
    return buf;
}

char* readdir(const char *pathname) {
    static size_t ptr = 0;
    static char *path = NULL;
    static node_t** entries;
    static size_t entry_count = 0;
    bool is_new = false;
    if (path == NULL || strcmp(path, pathname) != 0) {
        if (path != NULL)
            free(path);
        size_t len = strlen(pathname);
        path = malloc(len+1);
        strcpy(path, pathname);
        is_new = true;
    }

    if (!is_new) {
        if (ptr < entry_count) {
            return get_path(entries[ptr++]);
        } else {
            return NULL;
        }
    }

    node_t *node = trie;
    for (int i = 0; pathname[i] != '\0'; i++) {
        node = node->nodes[pathname[i]];
    }
    ptr = 0;
    entries = node->dir_entries;
    entry_count = node->number_of_entries;
    return readdir(pathname);
}

int16_t close(int32_t fd) {
    descriptors[fd]->used = false;
    if (descriptors[fd]->flags != READONLY) {
        pthread_mutex_unlock(add(descriptors[fd]->pathname, trie)->lock);
    }
    pthread_mutex_unlock(descriptors[fd]->lock);
    free(descriptors[fd]);
    return 0; // Indicates success
}

size_t write(int32_t fd, const char *buf, size_t count) {
    if (descriptors[fd]->flags == READONLY)
        return 0;

    entry_info_t *info = add(descriptors[fd]->pathname, trie);

    size_t ptr = 0;
    while (ptr < count) { int32_t block = get_empty_block();
        if (block == -1) { // No empty space
            return ptr;
        }
        size_t block_ptr = 0;
        while(ptr < count && block_ptr < BLOCK_SIZE) {
            memory[block]->memory[block_ptr] = buf[ptr];
            ptr++;
            block_ptr++;
        }
        memory[block]->last = block_ptr;
        pthread_mutex_unlock(memory[block]->lock);
        info->blocks[info->last] = (size_t) block;
        info->last++;
    }
    return ptr;
}

size_t read(int32_t fd, char *buf, size_t count) {
    if (descriptors[fd]->flags == WRITEONLY)
        return 0;

    entry_info_t *info = add(descriptors[fd]->pathname, trie);

    size_t ptr = 0;
    size_t outer_block_ptr = 0;
    size_t inner_block_ptr = 0;
    while (ptr < count && outer_block_ptr < info->last) {
        block_t *current_block = memory[info->blocks[outer_block_ptr]];
        if (inner_block_ptr == current_block->last) {
            inner_block_ptr = 0;
            outer_block_ptr++;
            continue;
        }
        buf[ptr] = current_block->memory[inner_block_ptr];
        inner_block_ptr++;
        ptr++;
    }
    return ptr;
}

void init_filesystem() {
    trie = new_node(DIR_T, '/');
    for (size_t i = 0; i < MEM_SIZE; i++) {
        memory[i] = new_block();
    }
}
