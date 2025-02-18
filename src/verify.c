#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ed25519.h"

int main(int argc, char **argv) {
    // Checking arguments
    if (argc < 4) {
        perror("[Usage] ./verify pkfile datafile sigfile\n"
               "Where :\n"
               "\tpkfile : public key file\n"
               "\tdatafile : file containing data\n"
               "\tsigfile : file containing signature\n");
        return 1;
    }
    char *pkfile = argv[1];
    char *datafile = argv[2];
    char *sigfile = argv[3];

    // Fetching key and signature
    uchar pk[32];
    load_file_buffer(pkfile, "", 32, (char *)pk);

    uchar sig[64];
    load_file_buffer(sigfile, "", 64, (char *)sig);

    // Reading data
    size_t fsize;
    char *data = read_data_file(datafile, &fsize);

    if (ed25519_verify(pk, data, fsize, sig))
        printf("ACCEPT\n");
    else
        printf("REJECT\n");

    free(data);
}
