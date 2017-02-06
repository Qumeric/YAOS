#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main() {
    init_filesystem();

    const size_t DESC_NUM = 16; // 32
    const size_t MAX_NAME_LEN = 16; //32
    const size_t MAX_DATA_LEN = 128; //8096

    int32_t fds[DESC_NUM];
    int32_t lens[DESC_NUM];
    for (int i = 0; i < DESC_NUM; i++) {
        size_t len = (size_t) (rand() % MAX_NAME_LEN + 1);
        char *data = malloc(len+1);
        rand_string(data, len);
        printf("Opening: %s\n", data);
        fds[i] = open(data, READWRITE);
        free(data);
    }

    for (int i = 0; i < DESC_NUM; i++) {
        size_t len = (size_t) (rand() % MAX_DATA_LEN + 1);
        char *data = malloc(len+1);
        rand_string(data, len);
        lens[i] = (int32_t) len;
        printf("%d writing: %s\n", i, data);
        write((uint32_t) fds[i], data, len);
        free(data);
    }

    for (int i = 0; i < DESC_NUM; i++) {
        char *buf = malloc(MAX_DATA_LEN);
        size_t len = read((uint32_t) fds[i], buf, (size_t) (lens[i] / 2));
        buf[len] = '\0';
        printf("%d reading: %s\n", i, buf);
        close(fds[i]);
        free(buf);
    }

    mkdir("test/");
    open("test/a", READWRITE);
    open("test/b", READWRITE);
    open("test/c", READWRITE);
    char *filename ;
    for (char *filename = readdir("test/"); filename != NULL; filename = readdir("test/")) {
        printf("content: %s\n", filename);
        free(filename);
    }
}