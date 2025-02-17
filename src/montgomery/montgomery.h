#ifndef MONTGOMERY_H
#define MONTGOMERY_H

#include <gmp.h>

typedef struct point {
    mpz_t x;
    mpz_t z;
} point;

void point_init(point *p);

void point_clear(point *p);

void point_normalize(point *p, mpz_t mod);

void point_print(point p);

/**
 * See section 5
 */
void cswap(int swap, point *p, point *q);

/**
 * See slide 15 / section 5
 * Set rop to p + q
 * @param dif is the value of p - q
 * @param mod is 2^255 - 19 for x25519
 */
void xadd(point *rop, point p, point q, point dif, mpz_t mod);

/**
 * See slide 16 / section 5
 * Set rop to [2]p
 * @param a24 Value of (A + 2) / 4
 * @param mod is 2^255 - 19 for x25519
 */
void xdbl(point *rop, point p, mpz_t a24, mpz_t mod);

/**
 * See slide 13 / section 5
 * Set rop to [m]p
 * p should be x-coord of a normalised point (z = 1)
 * @param a24 Value of (A + 2) / 4
 * @param mod is 2^255 - 19 for x25519
 */
void ladder(mpz_t rop, mpz_t m, mpz_t p, mpz_t a24, mpz_t mod);

/**
 * See slide 13 / section 5
 * Set rop to [m]p
 * p should be x-coord of a normalised point (z = 1)
 */
void curve25519_ladder(mpz_t rop, mpz_t m, mpz_t p);

#endif // MONTGOMERY_H
