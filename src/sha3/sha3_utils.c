#include "sha3_utils.h"

#include <assert.h>

void xorn(char *dest, char *a, char *b, size_t n) {
    for (size_t i = 0; i < n; ++i)
        dest[i] = a[i] ^ b[i];
}

uint64_t u64_from_chars(char *in) {
    uint64_t s = 0;
    for (int k = 0; k < 8; ++k)
        s |= ((uint64_t)in[k] & 0xFF) << (8 * k);
    return s;
}

void u64_to_chars(uint64_t x, char *out) {
    for (int k = 0; k < 8; ++k)
        out[k] = (x >> (8 * k)) & 0xFF;
}

char get_bit(char x, char i) {
    assert(0 <= i && i < 8);
    return (x >> i) & 1;
}

void set_bit(char *x, char i, char v) {
    assert(0 <= i && i < 8);

    char mask = 1 << i;
    if (v)
        // Set bit to 1
        *x |= mask;
    else
        // Set bit to 0
        *x &= ~mask;
}

uint64_t rotl_64_1(uint64_t x) {
    uint64_t msb = (x >> 63) & 1;
    return (x << 1) | msb;
}

uint64_t rotl_64(uint64_t x, size_t i) {
    assert(i < 64);
    if (i == 0)
        return x;
    return (x << i) | (x >> (64 - i));
}
