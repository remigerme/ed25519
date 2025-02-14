#include <stdio.h>
#include <string.h>

#include "ed25519.h"

int main(int argc, char **argv) {
    // Checking arguments
    if (argc < 4) {
        perror("[Usage] ./keygen pkfile datafile sigfile\n"
               "Where :\n"
               "\pkfile : public key file\n"
               "\tdatafile : file containing data\n"
               "\tsigfile : file containing signature\n");
        return 1;
    }
    char *pkfile = argv[1];
    char *datafile = argv[2];
    char *sigfile = argv[3];

    // Fetching key and signature
    uchar pk[32];
    load_file_buffer(pkfile, "", 32, pk);

    uchar sig[64];
    load_file_buffer(sigfile, "", 64, sig);

    // Reading data
    char *data = read_data_file(datafile);

    if (ed25519_verify(pk, data, sig))
        printf("ACCEPT\n");
    else
        printf("REJECT\n");

    free(data);
}
