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

void mpz_to_chars(mpz_t n, size_t n_bits, char *nb) {
    // Copy to avoid modifying input
    mpz_t temp;
    mpz_init_set(temp, n);

    for (size_t i = 0; i < n_bits; ++i) {
        nb[i] = (char)(mpz_get_ui(temp) & 0xFF);
        mpz_fdiv_q_2exp(temp, temp, 8);
    }

    mpz_clear(temp);
}
