//
// Created by chris on 6/18/21.
//

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <tuple>
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

std::tuple<int, char*> Util::HexToBytes(const std::string& hex) {
    int byteLength = hex.length() / 2;

    //TODO encapsulate this in a shared pointer so it will self free?
    char *bytes = new char[byteLength];


    for (int i = 0; i < hex.length(); i += 2) {
        int byteIdx = i == 0 ? i : i / 2;
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes[byteIdx] = byte;
    }

    return std::make_tuple(byteLength, bytes);
}
