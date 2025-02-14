#include <stdio.h>
#include <string.h>

#include "ed25519.h"

int main(int argc, char **argv) {
    // Checking argument
    if (argc < 2) {
        perror("[Usage] ./keygen prefix\n"
               "Where :\n"
               "\tprefix : keys will be saved in prefix.sk and prefix.pk\n");
        return 1;
    }
    char *prefix = argv[1];

    // Computing keys
    uchar sk[32];
    uchar pk[32];
    ed25519_keygen(sk, pk);

    // Saving keys to file
    save_buffer_file(sk, 32, prefix, ".sk");
    save_buffer_file(pk, 32, prefix, ".pk");
}
