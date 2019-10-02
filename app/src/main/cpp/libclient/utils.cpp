//
// Created by kottonye on 2019-10-02.
//

#include <string>
#include <sodium.h>
#include "utils.h"
#include "buffer.h"

void Utils::sha256(uint8_t* output, uint8_t * input, int inlen) {
    if (sodium_init() == -1) return;
    crypto_hash_sha256(output, input, inlen);
}

void Utils::ipv4_to_str(uint32_t ipv4, char* outstr) {
    std::string ipstr;
    ipstr.assign(50, '\0');
    char addr_str[257];
    sprintf(&ipstr[0], "%d.%d.%d.%d", ipv4 >> 24, ipv4 << 8 >> 24, ipv4 << 16 >> 24, ipv4 << 24 >> 24);
    strcpy(outstr, ipstr.c_str());
}