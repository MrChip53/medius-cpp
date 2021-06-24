//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_UTIL_H
#define MEDIUS_MUIS_SERVER_UTIL_H


#include <vector>
#include <string>

class Util {
public:
    static void fatal_error(const char *syscall);
    static void strtolower(char *str);
    static void *cmalloc(size_t size);
    static std::tuple<int, char*> HexToBytes(const std::string& hex);
    static std::vector<struct iovec> CreateIovec(void *packet, uint8_t response, uint16_t dataLength);
    static struct iovec CreateStaticIovec(void *packet, int dataLength);
    static std::string ToHex(const std::string& str);
};


#endif //MEDIUS_MUIS_SERVER_UTIL_H
