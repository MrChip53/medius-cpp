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

std::vector<char> Util::HexToBytes(const std::string& hex) {
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}
