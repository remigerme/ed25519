#include "ed25519.h"

#include "ed25519_utils.h"
#include "montgomery.h"
#include "sha3.h"

#include <gmp.h>
#include <stdlib.h>
#include <string.h>

void ed25519_keygen(uchar sk[32], uchar pk[32]) {
    // Generating private key - 32 random bytes
    random_bytes(sk, 32);

    // Deriving public key from private key
    uchar h[64];
    mpz_t s, A, p;
    mpz_inits(s, A, p, NULL);

    sha3_512((char *)sk, 32, (char *)h);

    decode_scalar_25519(h, s);

    mpz_set_ui(p, 9);
    curve25519_ladder(A, s, p);

    encode_u_coord_25519(A, pk);

    mpz_clears(s, A, p, NULL);
}

void ed25519_sign(uchar sk[32], uchar pk[32], char *data, uchar sig[64]) {}

int ed25519_verify(uchar pk[32], char *data, uchar sig[64]) {}
