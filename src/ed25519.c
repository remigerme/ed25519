#include "ed25519.h"

#include "ed25519_utils.h"

#include <gmp.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

void set_prime(mpz_t p) {
    mpz_set_ui(p, 1);
    mpz_mul_2exp(p, p, 255);
    mpz_sub_ui(p, p, 19);
}

void set_group_order(mpz_t L) {
    mpz_t temp;
    mpz_init(temp);
    mpz_set_ui(L, 1);
    mpz_mul_2exp(L, L, 252);
    mpz_set_str(temp, "27742317777372353535851937790883648493", 10);
    mpz_add(L, L, temp);
    mpz_clear(temp);
}

void set_initial_ed_point(mpz_t prime, mpz_t P[4]) {
    mpz_set_str(P[0],
                "15112221349535400772501151409588531511454012693041857206046113"
                "283949847762202",
                10);
    mpz_set_str(P[1],
                "46316835694926478169428394003475163141307993866256225615783033"
                "603165251855960",
                10);
    mpz_set_ui(P[2], 1);
    mpz_mul(P[3], P[0], P[1]);
    mpz_mod(P[3], P[3], prime);
}

void generate_public_key(uchar sk[32], uchar pk[32], uchar *h_snd_half,
                         mpz_t *a) {
    uchar h[64];
    mpz_t s, temp, prime, A[4], B[4];
    mpz_inits(s, temp, prime, NULL);
    ed_point_init(A);
    ed_point_init(B);
    set_prime(prime);
    set_initial_ed_point(prime, B);

    // Step 1
    SHA512(sk, 32, h);

    if (h_snd_half)
        memcpy(h_snd_half, &h[32], 32);

    // Step 2
    chars_to_mpz(h, 32, s);
    mpz_set_ui(temp, 1);
    mpz_mul_2exp(temp, temp, 254);
    mpz_sub_ui(temp, temp, 8);
    mpz_and(s, s, temp);

    mpz_add_ui(temp, temp, 8);
    mpz_ior(s, s, temp);

    if (a)
        mpz_set(*a, s);

    // Step 3
    ed_point_mul(s, B, prime, A);

    // Step 4
    ed_point_compress(A, prime, pk);

    mpz_clears(s, temp, prime, NULL);
    ed_point_clear(A);
    ed_point_clear(B);
}

void ed25519_keygen(uchar sk[32], uchar pk[32]) {
    // Generating private key - 32 random bytes
    random_bytes(sk, 32);

    // Deriving public key from private key
    generate_public_key(sk, pk, NULL, NULL);
}

void ed25519_sign(uchar sk[32], char *data, size_t data_size, uchar sig[64]) {
    mpz_t s, prime, L, r, k, A[4], B[4], Rp[4];
    mpz_inits(s, prime, L, r, k, NULL);

    ed_point_init(A);
    ed_point_init(B);
    ed_point_init(Rp);

    set_prime(prime);
    set_initial_ed_point(prime, B);
    set_group_order(L);

    // Step 1
    uchar pk[32];
    uchar prefix[32];
    generate_public_key(sk, pk, prefix, &s);

    // Step 2
    uchar *buf2 = (uchar *)malloc(32 + data_size);
    memcpy(buf2, prefix, 32);
    memcpy(&buf2[32], data, data_size);
    uchar rb[64];
    SHA512(buf2, 32 + data_size, rb);
    chars_to_mpz(rb, 64, r);
    free(buf2);

    // Step 3
    uchar R[32];
    mpz_mod(r, r, L);

    ed_point_mul(r, B, prime, Rp);
    ed_point_compress(Rp, prime, R);

    // Step 4
    uchar *buf4 = (uchar *)malloc(64 + data_size);
    memcpy(buf4, R, 32);
    memcpy(&buf4[32], pk, 32);
    memcpy(&buf4[64], data, data_size);
    uchar kb[64];
    SHA512(buf4, 64 + data_size, kb);
    chars_to_mpz(kb, 64, k);
    free(buf4);

    // Step 5
    mpz_mod(k, k, L);
    mpz_mul(k, k, s);
    mpz_add(k, k, r);
    mpz_mod(k, k, L); // k contains S at this point

    // Step 6
    memcpy(sig, R, 32);
    uchar S[32];
    mpz_to_chars(k, 32, S);
    memcpy(&sig[32], S, 32);

    ed_point_clear(A);
    ed_point_clear(B);
    ed_point_clear(Rp);

    mpz_clears(s, A, L, r, k, NULL);
}

/*
int ed25519_verify(uchar pk[32], char *data, size_t data_size, uchar sig[64]) {
    int res = 1;

    mpz_t R, S, A, k, L, p, left, right;
    mpz_inits(R, S, A, k, L, p, left, right, NULL);

    set_group_order(L);

    // Step 1
    decode_u_coord(sig, 255, R);
    decode_u_coord(&sig[32], 255, S);
    if (mpz_cmp(S, L) < 0)
        res = 0;
    decode_u_coord(pk, 255, A);

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
*/
