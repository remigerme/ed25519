#ifndef KECCAK_H
#define KECCAK_H

#include <stdlib.h>

#define B 200 // 1600 bits in bytes, for keccac_c
#define NR 24 // Number of rounds

/**
 * Use this function to prepare the buffer to use for pad10_1
 * @param x The desired padding alignment in bits
 * @param m The length of the string to pad in bits
 * @return Length of the padding in bits
 */
size_t get_padding_size(size_t x, size_t m);

/**
 * See section 5.1 p19.
 * @param x The desired padding alignment in bits
 * @param m The length of the string to pad in bits
 * @param in The string to pad. Padding bits will be added at the end. It is
 * caller's responsability to make sure enough memory has been allocated (use
 * get_padding_size to prepare the buffer)
 */
void pad10_1(size_t x, size_t m, char *in);

/**
 * See section 5.2 p20.
 * @param c The capacity of the scheme in bytes
 * @param N The input string
 * @param n_size_bits The size of the input string in bits
 * @param d The output length in bytes
 * @param digest The output buffer of size d bytes
 */
void keccak_c(size_t c, char *N, size_t n_size_bits, size_t d, char *digest);

#endif // KECCAK_H
