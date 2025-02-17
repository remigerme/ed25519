#ifndef SHA3_H
#define SHA3_H

#include <stdlib.h>

/**
 * See section 6.1 p20.
 * @param M Buffer to hash
 * @param n Size of the buffer to hash in bytes
 * @param digest SHA3-224 hash of size 28 bytes (=224 bits)
 */
void sha3_224(char *M, size_t n, char digest[28]);

/**
 * See section 6.1 p20.
 * @param M Buffer to hash
 * @param n Size of the buffer to hash in bytes
 * @param digest SHA3-256 hash of size 32 bytes (=256 bits)
 */
void sha3_256(char *M, size_t n, char digest[32]);

/**
 * See section 6.1 p20.
 * @param M Buffer to hash
 * @param n Size of the buffer to hash in bytes
 * @param digest SHA3-384 hash of size 48 bytes (=384 bits)
 */
void sha3_384(char *M, size_t n, char digest[48]);

/**
 * See section 6.1 p20.
 * @param M Buffer to hash
 * @param n Size of the buffer to hash in bytes
 * @param digest SHA3-512 hash of size 64 bytes (=512 bits)
 */
void sha3_512(char *M, size_t n, char digest[64]);

/**
 * See section 6.2 p20.
 * @param M Buffer to use for the XOF
 * @param n Size of the input buffer in bytes
 * @param d Number of bytes of the output length
 * @param digest SHAKE128 XOF output of size d bytes, not null terminated (!)
 */
void shake128(char *M, size_t n, size_t d, char *digest);

/**
 * See section 6.2 p20.
 * @param M Buffer to use for the XOF
 * @param n Size of the input buffer in bytes
 * @param d Number of bytes of the output length
 * @param digest SHAKE256 XOF output of size d bytes, not null terminated (!)
 */
void shake256(char *M, size_t n, size_t d, char *digest);

#endif // SHA3_H
