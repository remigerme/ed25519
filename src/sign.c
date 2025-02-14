#include <stdio.h>
#include <string.h>

#include "ed25519.h"

int main(int argc, char **argv) {
    // Checking arguments
    if (argc < 4) {
        perror("[Usage] ./keygen prefix datafile sigfile\n"
               "Where :\n"
               "\tprefix : keys will be loaded from prefix.sk and prefix.pk\n"
               "\tdatafile : file containing data to sign\n"
               "\tsigfile : signature will be saved to sigfile\n");
        return 1;
    }
    char *prefix = argv[1];
    char *datafile = argv[2];
    char *sigfile = argv[3];

    // Fetching keys
    uchar sk[32];
    uchar pk[32];
    load_file_buffer(prefix, ".sk", 32, sk);
    load_file_buffer(prefix, ".pk", 32, pk);

    // Fetching data
    char *data = read_data_file(datafile);

    // Computing signature
    uchar sig[64];
    ed25519_sign(sk, pk, data, sig);

    // Writing signature
    save_buffer_file(sig, sigfile, "", 64);
}
