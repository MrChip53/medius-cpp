//
// Created by chris on 6/18/21.
//

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <cstring>
#include <bits/types/struct_iovec.h>
#include <ios>
#include <iomanip>
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
    char *bytes = static_cast<char *>(malloc(sizeof(char) * byteLength));


    for (int i = 0; i < hex.length(); i += 2) {
        int byteIdx = i == 0 ? i : i / 2;
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), nullptr, 16);
        bytes[byteIdx] = byte;
    }

    return std::make_tuple(byteLength, bytes);
}

std::vector<struct iovec> Util::CreateIovec(void* packet, uint8_t response, uint16_t dataLength) {
    std::vector<struct iovec> iovs;

    struct iovec iov;

    iov.iov_len = dataLength + 3;
    char *buffer = static_cast<char *>(malloc(sizeof(char) * (dataLength + 3)));
    memset(buffer, 0, dataLength + 3);
    buffer[0] = response;
    buffer[1] = (uint16_t)dataLength;
    memcpy(&buffer[3], packet, dataLength);
    free(packet);

    iov.iov_base = buffer;

    iovs.push_back(iov);

    return iovs;
}

std::vector<struct iovec> Util::CreateMediusIovec(void* packet, uint16_t dataLength, uint8_t mediusClass, uint8_t mediusId) {
    std::vector<struct iovec> iovs;

    struct iovec iov;

    iov.iov_len = dataLength + 5;
    char *buffer = static_cast<char *>(malloc(sizeof(char) * (dataLength + 5)));
    memset(buffer, 0, dataLength + 5);
    buffer[0] = 0x0a;
    uint16_t mediusSize = dataLength + 2;
    memcpy(&buffer[1], &mediusSize, sizeof(uint16_t));
    buffer[3] = mediusClass;
    buffer[4] = mediusId;
    memcpy(&buffer[5], packet, dataLength);
    free(packet);

    iov.iov_base = buffer;

    iovs.push_back(iov);

    return iovs;
}

struct iovec Util::CreateStaticIovec(void *packet, int dataLength) {
    struct iovec iov;

    iov.iov_len = dataLength;
    iov.iov_base = packet;

    return iov;
}

std::string Util::ToHex(const std::string& str) {
        std::ostringstream ret;
        for (char i : str)
            ret << std::hex
                << std::setfill('0')
                << std::setw(2)
                << (int)i;
        return ret.str();
}

std::string Util::ToHexFixedLength(const std::string& str, int length) {
    std::ostringstream ret;
    if (str.length() < length) {
        for (char i : str)
            ret << std::hex
                << std::setfill('0')
                << std::setw(2)
                << (int) i;

        for (int i = 0; i < length - str.length(); i++) {
            ret << "00";
        }
    } else {
        for (int i = 0; i < length; i++)
            ret << std::hex
                << std::setfill('0')
                << std::setw(2)
                << (int)str[i];
    }
    return ret.str();
}

std::string Util::Int32ToHex(const int number) {
    std::ostringstream intstr;
    intstr << std::hex << std::setfill('0') << std::setw(8) << number;

    return intstr.str();
}

std::string Util::ReverseHexString(const std::string& s1)
{
    if (s1.size() & 1) return std::string(); // reject odd-length strings

    std::string s = s1;

    std::reverse(begin(s), end(s));

    for (int i = 0; i + 1 < s.size(); i += 2) {
        std::swap(s[i], s[i + 1]);
    }

    return s;
}
