#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ed25519.h"

int main(int argc, char **argv) {
    // Checking arguments
    if (argc < 4) {
        perror("[Usage] ./sign prefix datafile sigfile\n"
               "Where :\n"
               "\tprefix : keys will be loaded from prefix.sk and prefix.pk\n"
               "\tdatafile : file containing data to sign\n"
               "\tsigfile : signature will be saved to sigfile\n");
        return 1;
    }
    char *prefix = argv[1];
    char *datafile = argv[2];
    char *sigfile = argv[3];

    // Fetching key
    uchar sk[32];
    load_file_buffer(prefix, ".sk", 32, (char *)sk);

    // Fetching data
    size_t fsize;
    char *data = read_data_file(datafile, &fsize);

    // Computing signature
    uchar sig[64];
    ed25519_sign(sk, data, fsize, sig);

    // Writing signature
    save_buffer_file((char *)sig, 64, sigfile, "");
}
