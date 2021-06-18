//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_UTIL_H
#define MEDIUS_MUIS_SERVER_UTIL_H


class Util {
public:
    static void fatal_error(const char *syscall);
    static void strtolower(char *str);
    static void *cmalloc(size_t size);
};


#endif //MEDIUS_MUIS_SERVER_UTIL_H
