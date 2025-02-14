#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

void random_bytes(uchar *buf, size_t size);

void save_buffer_file(char *buf, size_t size, char *prefix, char *ext);

void load_file_buffer(char *prefix, char *ext, size_t size, char *buf);

char *read_data_file(char *datafile);

/**
 * Retrieve file size
 * https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
 * @param fd File descriptor to the studied file
 * @return File size in bytes
 */
long get_file_size(FILE *fd);

#endif // UTILS_H
