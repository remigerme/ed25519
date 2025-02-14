#ifndef ED25519_H
#define ED25519_H

#include "utils.h"

void ed25519_keygen(uchar sk[32], uchar pk[32]);

void ed25519_sign(uchar sk[32], uchar pk[32], char *data, uchar sig[64]);

int ed25519_verify(uchar pk[32], char *data, uchar sig[64]);

#endif // ED25519_H
