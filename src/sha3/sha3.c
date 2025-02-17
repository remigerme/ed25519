#include "sha3.h"

#include <stdlib.h>
#include <string.h>

#include "keccak_c.h"
#include "sha3_utils.h"

// Returns a new array with the two bits suffix 01
char *prepare_sha3(char *M, size_t n) {
    char *N = (char *)malloc(sizeof(char) * (n + 1));
    memcpy(N, M, n);
    set_bit(&N[n], 0, 0);
    set_bit(&N[n], 1, 1);
    return N;
}

void sha3_224(char *M, size_t n, char digest[28]) {
    char *N = prepare_sha3(M, n);
    keccak_c(56, N, 8 * n + 2, 28, digest);
    free(N);
}

void sha3_256(char *M, size_t n, char digest[32]) {
    char *N = prepare_sha3(M, n);
    keccak_c(64, N, 8 * n + 2, 32, digest);
    free(N);
}

void sha3_384(char *M, size_t n, char digest[48]) {
    char *N = prepare_sha3(M, n);
    keccak_c(96, N, 8 * n + 2, 48, digest);
    free(N);
}

void sha3_512(char *M, size_t n, char digest[64]) {
    char *N = prepare_sha3(M, n);
    keccak_c(128, N, 8 * n + 2, 64, digest);
    free(N);
}

// Returns a new array with the 4 bits suffix 1111
char *prepare_shake(char *M, size_t n) {
    char *N = (char *)malloc(sizeof(char) * (n + 1));
    memcpy(N, M, n);
    set_bit(&N[n], 0, 1);
    set_bit(&N[n], 1, 1);
    set_bit(&N[n], 2, 1);
    set_bit(&N[n], 3, 1);
    return N;
}

void shake128(char *M, size_t n, size_t d, char *digest) {
    char *N = prepare_shake(M, n);
    keccak_c(32, N, 8 * n + 4, d, digest);
    free(N);
}

void shake256(char *M, size_t n, size_t d, char *digest) {
    char *N = prepare_shake(M, n);
    keccak_c(64, N, 8 * n + 4, d, digest);
    free(N);
}
