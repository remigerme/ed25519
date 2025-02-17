#ifndef SHA3_UTILS_H
#define SHA3_UTILS_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Xor n bytes of two arrays a and b and store the result into dest
 * @param dest Array where to store the result
 * @param a First array to xor
 * @param b Second array to xor
 * @param n Number of bytes to xor
 */
void xorn(char *dest, char *a, char *b, size_t n);

/**
 * This functions uses little endian mode
 * @param in Pointer to the first char to read (will read 8 chars)
 * @return u64 obtained after concatenating all 8 chars
 */
uint64_t u64_from_chars(char *in);

/**
 * This function uses little endian mode
 * @param x u64 to convert into an array of 8 chars
 * @param out Destination array
 */
void u64_to_chars(uint64_t x, char *out);

/**
 * @param x The value we want to get the i-th bit of
 * @param i Index of the bit (0 : LSB, 7 : MSB)
 * @return The i-th bit of x (0 : LSB)
 */
char get_bit(char x, char i);

/**
 * @param x The byte to modify in place
 * @param i Index of the bit (0 : LSB, 7 : MSB)
 * @param v The value of the bit (0 or 1)
 */
void set_bit(char *x, char i, char v);

/**
 * @param x Int to be rotated
 * @param i Number of bits to rotate (0 <= i < 64)
 * @return x rotated by i bits to the left
 */
uint64_t rotl_64(uint64_t x, size_t i);

#endif // SHA3_UTILS_H
