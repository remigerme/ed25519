#include "ed25519.h"

#include "ed25519_utils.h"
#include "montgomery.h"
#include "sha3.h"

#include <gmp.h>
#include <stdlib.h>
#include <string.h>

void ed25519_keygen(uchar sk[32], uchar pk[32]) {
    // Generating private key - 32 random bytes
    random_bytes(sk, 32);

    // Deriving public key from private key
    uchar h[64];
    mpz_t s, A, p;
    mpz_inits(s, A, p, NULL);

    sha3_512((char *)sk, 32, (char *)h);

    decode_scalar_25519(h, s);

    mpz_set_ui(p, 9);
    curve25519_ladder(A, s, p);

    encode_u_coord_25519(A, pk);

    mpz_clears(s, A, p, NULL);
}

void ed25519_sign(uchar sk[32], char *data, size_t data_size, uchar sig[64]) {
    mpz_t s, A, p, L, r, k, Rm;
    mpz_inits(s, A, p, L, r, k, Rm, NULL);

    // Step 1
    uchar pk[32];
    uchar h[64];
    sha3_512((char *)sk, 32, (char *)h);
    decode_scalar_25519(h, s);
    mpz_set_ui(p, 9);
    curve25519_ladder(A, s, p);
    encode_u_coord_25519(A, pk);

    // Step 2
    char *buf2 = (char *)malloc(32 + data_size);
    memcpy(buf2, &h[32], 32);
    memcpy(&buf2[32], data, data_size);
    uchar rb[64];
    sha3_512(buf2, 32 + data_size, (char *)rb);
    decode_le(rb, 64 * 8, r);
    free(buf2);

    // Step 3
    uchar R[32];
    // Initializing L
    mpz_set_ui(L, 1);
    mpz_mul_2exp(L, L, 252);
    // using Rm as a temp var
    mpz_set_str(Rm, "27742317777372353535851937790883648493", 10);
    mpz_add(L, L, Rm);
    mpz_mod(r, r, L);
    curve25519_ladder(Rm, r, p);
    encode_u_coord_25519(Rm, R);

    // Step 4
    char *buf4 = (char *)malloc(64 + data_size);
    memcpy(buf4, R, 32);
    memcpy(&buf4[32], pk, 32);
    memcpy(&buf4[64], data, data_size);
    uchar kb[64];
    sha3_512(buf4, 64 + data_size, (char *)kb);
    decode_le(kb, 64 * 8, k);
    free(buf4);

    // Step 5
    mpz_mod(k, k, L);
    mpz_mul(k, k, s);
    mpz_add(k, k, r);
    mpz_mod(k, k, L); // k contains S at this point

    // Step 6
    memcpy(sig, R, 32);
    char S[32];
    mpz_to_chars(k, 32, S);
    memcpy(&sig[32], S, 32);

    mpz_clears(s, A, p, L, r, k, Rm, NULL);
}

int ed25519_verify(uchar pk[32], char *data, size_t data_size, uchar sig[64]) {
    int res = 1;

    mpz_t R, S, A, k, L, p, left, right;
    mpz_inits(R, S, A, k, L, p, left, right, NULL);

    // Initializing L
    mpz_set_ui(L, 1);
    mpz_mul_2exp(L, L, 252);
    // using A as a temp var
    mpz_set_str(A, "27742317777372353535851937790883648493", 10);
    mpz_add(L, L, A);

    // Step 1
    decode_u_coord(sig, 32, R);
    decode_u_coord(&sig[32], 32, S);
    if (mpz_cmp(S, L) < 0)
        res = 0;
    decode_u_coord(pk, 32, A);

    // Step 2
    char *buf2 = (char *)malloc(64 + data_size);
    memcpy(buf2, R, 32);
    memcpy(&buf2[32], pk, 32);
    memcpy(&buf2[64], data, data_size);
    uchar kb[64];
    sha3_512(buf2, 64 + data_size, (char *)kb);
    decode_le(kb, 64 * 8, k);
    free(buf2);

    // Step 3
    // Computing left term
    mpz_set_ui(p, 9);
    curve25519_ladder(left, S, p);
    // Computing right term
    curve25519_ladder(right, k, A);
    mpz_add(right, right, R);

    if (mpz_cmp(left, right) != 0)
        res = 0;

    mpz_clears(R, S, A, k, L, p, left, right, NULL);

    return res;
}
