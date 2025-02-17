#include "keccak_c.h"

#include <assert.h>

#include "keccak_rho.h"
#include "sha3_utils.h"
#include "sponge.h"

size_t get_padding_size(size_t x, size_t m) {
    int j = (-(int)m - 2) % (int)x;
    if (j < 0)
        j += x;
    return j + 2;
}

void pad10_1(size_t x, size_t m, char *in) {
    // Step 1
    size_t j = get_padding_size(x, m) - 2;

    // Step 2
    // Because of padding for SHA3 / SHAKE functions, there are only two
    // expected cases as we read full bytes in inputs
    assert(m % 8 == 2 || m % 8 == 4);

    size_t current_byte = m / 8;
    char current_bit = m % 8;

    set_bit(&in[current_byte], current_bit, 1);
    while (j > 0) {
        current_byte += (current_bit + 1) / 8;
        current_bit = (current_bit + 1) % 8;
        set_bit(&in[current_byte], current_bit, 0);
        --j;
    }
    current_byte += (current_bit + 1) / 8;
    current_bit = (current_bit + 1) % 8;
    set_bit(&in[current_byte], current_bit, 1);
}

// Wrapper around keccac_rho for keccac_c
// Just specifiying the number of rounds
// Note : keccac_kho only supports b = 200 bytes (1600 bits) for now
void f(size_t b, char *in, char *out) {
    (void)b; // To remove the unused paramater warning for now
    keccak_rho(NR, in, out);
}

void keccak_c(size_t c, char *N, size_t n_size_bits, size_t d, char *digest) {
    sponge(B, &f, &pad10_1, B - c, N, n_size_bits, d, digest);
}
