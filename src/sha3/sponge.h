#ifndef SPONGE_H
#define SPONGE_H

#include <stdlib.h>

/**
 * Check section 4 - sponge construction, algorithm 8.
 * @param b Number of bytes of the state
 * @param f Function f called using f(b, in, out)
 * @param pad Function pad called using pad(x, m, buf_to_pad), x and m in bits
 * @param r Rate of the sponge schema in bytes
 * @param N Input buffer (not null-terminated)
 * @param n_size_bits Size of the input buffer in BITS
 * @param d Length of the output string in bytes
 * @param digest The output buffer of size d bytes
 */
void sponge(size_t b, void (*f)(size_t, char *, char *),
            void (*pad)(size_t, size_t, char *), size_t r, char *N,
            size_t n_size_bits, size_t d, char *digest);

#endif // SPONGE_H
