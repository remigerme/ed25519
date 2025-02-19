#include "ed25519_utils.h"

#include <stdio.h>
#include <string.h>

void random_bytes(uchar *buf, size_t size) {
    FILE *fd = fopen("/dev/urandom", "rb");
    if (fd == NULL) {
        perror("Couldn't open /dev/urandom\n");
        exit(EXIT_FAILURE);
    }
    if (fread(buf, 1, size, fd) < size) {
        fprintf(stderr, "Couldn't read %lu bytes from /dev/urandom\n", size);
        exit(EXIT_FAILURE);
    }
    fclose(fd);
}

void save_buffer_file(char *buf, size_t size, char *prefix, char *ext) {
    size_t sp = strlen(prefix);
    size_t se = strlen(ext);
    char filename[sp + se + 1];
    filename[0] = 0; // empty string !

    strcat(filename, prefix);
    strcat(filename, ext);

    FILE *fd = fopen(filename, "wb");
    if (fd == NULL) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    if (fwrite(buf, 1, size, fd) < size) {
        fprintf(stderr, "Couldn't write %lu bytes to %s\n", size, filename);
        exit(EXIT_FAILURE);
    }
    fclose(fd);
}

void load_file_buffer(char *prefix, char *ext, size_t size, char *buf) {
    size_t sp = strlen(prefix);
    size_t se = strlen(ext);
    char filename[sp + se + 1];
    filename[0] = 0; // empty string !

    strcat(filename, prefix);
    strcat(filename, ext);

    FILE *fd = fopen(filename, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    if (fread(buf, 1, size, fd) < size) {
        fprintf(stderr, "Couldn't read %lu bytes from %s\n", size, filename);
        exit(EXIT_FAILURE);
    }
    fclose(fd);
}

char *read_data_file(char *datafile, size_t *fsize) {
    FILE *fd = fopen(datafile, "rb");
    *fsize = get_file_size(fd);

    char *data = (char *)malloc(*fsize);
    if (data == NULL) {
        fprintf(stderr, "Couldn't allocate %lu bytes\n", *fsize);
        exit(EXIT_FAILURE);
    }
    if (fread(data, 1, *fsize, fd) < *fsize) {
        fprintf(stderr, "Couldn't read %lu bytes from %s\n", *fsize, datafile);
        exit(EXIT_FAILURE);
    }
    fclose(fd);

    return data;
}

long get_file_size(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    return size;
}

void mpz_to_chars(mpz_t n, size_t n_bytes, uchar *nb) {
    // Copy to avoid modifying input
    mpz_t temp;
    mpz_init_set(temp, n);

    for (size_t i = 0; i < n_bytes; ++i) {
        nb[i] = (uchar)(mpz_get_ui(temp) & 0xFF);
        mpz_fdiv_q_2exp(temp, temp, 8);
    }

    mpz_clear(temp);
}

void chars_to_mpz(uchar *nb, size_t n_bytes, mpz_t n) {
    mpz_set_ui(n, 0);
    for (int i = (int)n_bytes - 1; i >= 0; --i) {
        mpz_mul_2exp(n, n, 8);
        mpz_add_ui(n, n, nb[i]);
    }
}

void ed_point_init(mpz_t P[4]) { mpz_inits(P[0], P[1], P[2], P[3], NULL); }

void ed_point_clear(mpz_t P[4]) { mpz_clears(P[0], P[1], P[2], P[3], NULL); }

void ed_point_set(mpz_t P[4], mpz_t out[4]) {
    mpz_set(out[0], P[0]);
    mpz_set(out[1], P[1]);
    mpz_set(out[2], P[2]);
    mpz_set(out[3], P[3]);
}

void ed_point_add(mpz_t P[4], mpz_t Q[4], mpz_t prime, mpz_t out[4]) {
    mpz_t A, B, C, D, E, F, G, H, d;
    mpz_inits(A, B, C, D, E, F, G, H, d, NULL);
    mpz_set_str(d,
                "37095705934669439343138083508754565189542113879843219016388785"
                "533085940283555",
                10);

    mpz_sub(A, P[1], P[0]);
    mpz_sub(B, Q[1], Q[0]); // B temp
    mpz_mul(A, A, B);
    mpz_mod(A, A, prime);
    mpz_add(B, P[1], P[0]);
    mpz_add(C, Q[1], Q[0]); // C temp
    mpz_mul(B, B, C);
    mpz_mod(B, B, prime);

    mpz_mul_ui(C, P[3], 2);
    mpz_mul(C, C, Q[3]);
    mpz_mul(C, C, d);
    mpz_mod(C, C, prime);
    mpz_mul_ui(D, P[2], 2);
    mpz_mul(D, D, Q[2]);
    mpz_mod(D, D, prime);

    mpz_sub(E, B, A);
    mpz_sub(F, D, C);
    mpz_add(G, D, C);
    mpz_add(H, B, A);

    mpz_mul(out[0], E, F);
    mpz_mul(out[1], G, H);
    mpz_mul(out[2], F, G);
    mpz_mul(out[3], E, H);

    mpz_clears(A, B, C, D, E, F, G, H, d, NULL);
}

void ed_point_mul(mpz_t n, mpz_t P[4], mpz_t prime, mpz_t out[4]) {
    mpz_t s, Q[4];
    mpz_init_set(s, n);
    ed_point_init(Q);
    ed_point_set(P, Q);

    mpz_set_ui(out[0], 0);
    mpz_set_ui(out[1], 1);
    mpz_set_ui(out[2], 1);
    mpz_set_ui(out[3], 0);

    while (mpz_cmp_ui(s, 0) > 0) {
        if (mpz_tstbit(s, 0))
            ed_point_add(out, Q, prime, out);
        ed_point_add(Q, Q, prime, Q);
        mpz_div_2exp(s, s, 1);
    }

    mpz_clear(s);
    ed_point_clear(Q);
}

void ed_point_compress(mpz_t P[4], mpz_t prime, uchar buf[32]) {
    mpz_t zinv, x, y;
    mpz_inits(zinv, x, y, NULL);

    mpz_invert(zinv, P[2], prime);

    mpz_mul(x, P[0], zinv);
    mpz_mod(x, x, prime);

    mpz_mul(y, P[1], zinv);
    mpz_mod(y, y, prime);

    mpz_set_ui(zinv, 1); // zinv no longer needed
    mpz_and(x, x, zinv);
    mpz_mul_2exp(x, x, 255);

    mpz_ior(y, y, x);
    mpz_to_chars(y, 32, buf);

    mpz_clears(zinv, x, y, NULL);
}
