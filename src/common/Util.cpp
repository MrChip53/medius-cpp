//
// Created by chris on 6/18/21.
//

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "Util.h"

void Util::fatal_error(const char *syscall) {
    perror(syscall);
    exit(1);
}

void Util::strtolower(char *str) {
    for (; *str; ++str)
        *str = (char)tolower(*str);
}

void *Util::cmalloc(size_t size) {
    void *buf = malloc(size);
    if (!buf) {
        fprintf(stderr, "Fatal error: unable to allocate memory.\n");
        exit(1);
    }
    return buf;
}
