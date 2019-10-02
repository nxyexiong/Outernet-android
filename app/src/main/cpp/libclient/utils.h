//
// Created by kottonye on 2019-10-02.
//

#ifndef OUTERNET_UTILS_H
#define OUTERNET_UTILS_H

#include <stdint.h>

class Utils {
public:
    static void sha256(uint8_t* output, uint8_t * input, int inlen);
    static void ipv4_to_str(uint32_t ipv4, char* outstr);
};

#endif //OUTERNET_UTILS_H
