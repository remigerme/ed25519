#ifndef ED25519_UTILS_H
#define ED25519_UTILS_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

void random_bytes(uchar *buf, size_t size);

void save_buffer_file(char *buf, size_t size, char *prefix, char *ext);

void load_file_buffer(char *prefix, char *ext, size_t size, char *buf);

char *read_data_file(char *datafile, size_t *fsize);

/**
 * Retrieve file size
 * https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
 * @param fd File descriptor to the studied file
 * @return File size in bytes
 */
long get_file_size(FILE *fd);

void mpz_to_chars(mpz_t n, size_t n_bytes, uchar *nb);
void chars_to_mpz(uchar *nb, size_t n_bytes, mpz_t n);

void ed_point_init(mpz_t P[4]);
void ed_point_clear(mpz_t P[4]);
void ed_point_set(mpz_t P[4], mpz_t out[4]);

int ed_point_eq(mpz_t P[4], mpz_t Q[4], mpz_t prime);

void ed_point_add(mpz_t P[4], mpz_t Q[4], mpz_t prime, mpz_t out[4]);
void ed_point_mul(mpz_t n, mpz_t P[4], mpz_t prime, mpz_t out[4]);

void ed_point_compress(mpz_t P[4], mpz_t prime, uchar buf[32]);
int ed_point_decompress(uchar buf[32], mpz_t prime, mpz_t P[4]);
int ed_point_recover_x(mpz_t y, mpz_t sign, mpz_t prime, mpz_t x);

#endif // ED25519_UTILS_H
