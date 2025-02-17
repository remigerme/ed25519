#ifndef KECCAC_RHO_H
#define KECCAC_RHO_H

#include <stdint.h>
#include <stdlib.h>

/**
 * See algorithm 7 p17.
 * @param nr Number of rounds
 * @param in Padded array of 200 bytes (=1600 bits)
 * @param out Output array of 200 bytes (=1600 bits)
 */
void keccak_rho(size_t nr, char *in, char *out);

#endif // KECCAC_RHO_H
