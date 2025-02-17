#ifndef MONTGOMERY_IO_H
#define MONTGOMERY_IO_H

#include <gmp.h>

typedef unsigned char uchar;

/**
 * Reads n bytes (=n*2 hex characters) from in and save the result in out
 */
void hextouchars(char *in, int n, uchar *out);

/**
 * See section 5
 */
void decode_le(uchar *b, int n_bits, mpz_t decoded);

/**
 * See section 5
 * For now, only supports curve25519.
 */
void decode_u_coord(uchar ub[32], int n_bits, mpz_t u);

/**
 * See section 5
 */
void encode_u_coord(mpz_t u, int n_bits, mpz_t prime, uchar *ub);
void encode_u_coord_25519(mpz_t u, int n_bits, uchar *ub);

/**
 * See section 5
 */
void decode_scalar_25519(uchar kb[32], mpz_t decoded);

#endif // MONTGOMERY_IO_H
