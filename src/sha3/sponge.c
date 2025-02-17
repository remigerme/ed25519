#include "sponge.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "keccak_c.h"
#include "sha3_utils.h"

void sponge(size_t b, void (*f)(size_t, char *, char *),
            void (*pad)(size_t, size_t, char *), size_t r, char *N,
            size_t n_size_bits, size_t d, char *digest) {
    // Step 1
    size_t padding_size = get_padding_size(8 * r, n_size_bits);
    assert((n_size_bits + padding_size) % (8 * r) == 0);
    size_t p_size = (n_size_bits + padding_size) / 8;

    char *P = (char *)malloc(p_size);
    // Copy n_size_bits of N in P
    memcpy(P, N, n_size_bits / 8);               // whole bytes
    for (size_t i = 0; i < n_size_bits % 8; ++i) // left bits
        set_bit(&P[n_size_bits / 8], i, get_bit(N[n_size_bits / 8], i));

    pad(8 * r, n_size_bits, P);

    // Step 2
    size_t n = p_size / r;

    // Step 3
    size_t c = b - r;

    // Step 4 will be done in step 6
    char Pi[b];
    for (size_t i = 0; i < c; ++i)
        Pi[r + i] = 0;

    // Step 5
    char S[b];
    for (size_t i = 0; i < b; ++i)
        S[i] = 0;
    char S_out[b]; // Used to store the result of f

    // Step 6
    for (size_t i = 0; i < n; ++i) {
        memcpy(Pi, &P[i * r], r);
        xorn(S, S, Pi, b);
        f(b, S, S_out);
        memcpy(S, S_out, b);
    }

    // Step 7
    size_t nz_max = ceil((double)d / (double)r) * r;
    char *Z = (char *)malloc(nz_max);
    size_t nz = 0;

    while (nz < d) {
        // Step 8
        memcpy(&Z[nz], S, r);
        nz += r;

        // Step 10
        f(b, S, S_out);
        memcpy(S, S_out, b);
    }

    // Step 9
    memcpy(digest, Z, d);

    free(P);
    free(Z);
}
