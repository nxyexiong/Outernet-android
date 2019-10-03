//
// Created by kottonye on 2019-10-02.
//

#ifndef OUTERNET_UTILS_H
#define OUTERNET_UTILS_H

#include <stdint.h>

class Utils {
public:
    static void ipv4_to_str(uint32_t ipv4, char* outstr, bool is_bigend);
};

#endif //OUTERNET_UTILS_H
