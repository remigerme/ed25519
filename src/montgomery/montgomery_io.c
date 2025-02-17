#include "montgomery_io.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void hextouchars(char *in, int n, uchar *out) {
    assert((int)strlen(in) == 2 * n);
    char buf[3];
    buf[2] = '\0';
    char *end;
    for (int i = 0; i < n; ++i) {
        buf[0] = in[2 * i];
        buf[1] = in[2 * i + 1];
        out[i] = strtoul(buf, &end, 16);
        assert(buf + 2 == end);
    }
}

void decode_le(uchar *bs, int n_bits, mpz_t decoded) {
    mpz_set_ui(decoded, 0);
    for (int i = (n_bits + 7) / 8 - 1; i >= 0; --i) {
        mpz_mul_2exp(decoded, decoded, 8);
        mpz_add_ui(decoded, decoded, bs[i]);
    }
}

void decode_u_coord(uchar ub[32], int n_bits, mpz_t u) {
    // Specific to 25519 curve
    assert(n_bits == 255);

    uchar u_list[32];
    memcpy(u_list, ub, 32);

    if (n_bits % 8)
        u_list[31] &= (1 << (n_bits % 8)) - 1;

    decode_le(u_list, n_bits, u);
}

void encode_u_coord(mpz_t u, int n_bits, mpz_t prime, uchar *ub) {
    // Copy to avoid modify input
    mpz_t temp;
    mpz_init(temp);

    mpz_mod(temp, u, prime);

    for (int i = 0; i < (n_bits + 7) / 8; i++) {
        ub[i] = (uchar)(mpz_get_ui(temp) & 0xFF);
        mpz_fdiv_q_2exp(temp, temp, 8);
    }

    mpz_clear(temp);
}

void encode_u_coord_25519(mpz_t u, int n_bits, uchar *ub) {
    mpz_t prime;
    mpz_init_set_ui(prime, 1);
    mpz_mul_2exp(prime, prime, 255);
    mpz_sub_ui(prime, prime, 19);

    encode_u_coord(u, n_bits, prime, ub);

    mpz_clear(prime);
}

void decode_scalar_25519(uchar kb[32], mpz_t decoded) {
    uchar k_list[32];
    memcpy(k_list, kb, 32);

    k_list[0] &= 248;
    k_list[31] &= 127;
    k_list[31] |= 64;

    decode_le(k_list, 255, decoded);
}
