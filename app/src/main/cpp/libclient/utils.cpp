//
// Created by kottonye on 2019-10-02.
//

#include <string>
#include <sodium.h>
#include "utils.h"
#include "buffer.h"

void Utils::ipv4_to_str(uint32_t ipv4, char* outstr, bool is_bigend) {
    std::string ipstr;
    ipstr.assign(50, '\0');
    char addr_str[257];
    if (is_bigend)
        sprintf(&ipstr[0], "%d.%d.%d.%d", ipv4 << 24 >> 24, ipv4 << 16 >> 24, ipv4 << 8 >> 24, ipv4 >> 24);
    else
        sprintf(&ipstr[0], "%d.%d.%d.%d", ipv4 >> 24, ipv4 << 8 >> 24, ipv4 << 16 >> 24, ipv4 << 24 >> 24);
    strcpy(outstr, ipstr.c_str());
}