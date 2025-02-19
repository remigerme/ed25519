#include "ed25519.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void handle_test(char *name) {
    uchar sk[32], pk[32], expected_pk[32];
    char path[1024] = "tests/test-";
    strcat(path, name);

    load_file_buffer(path, "/key.sk.bin", 32, (char *)sk);
    load_file_buffer(path, "/key.pk.bin", 32, (char *)expected_pk);

    generate_public_key(sk, pk, NULL);

    for (int i = 0; i < 32; ++i)
        assert(pk[i] == expected_pk[i]);

    printf("Generated public key successfully for %s\n", name);
}

int main() {
    handle_test("1");
    handle_test("2");
    handle_test("3");
    handle_test("1024");
    handle_test("sha");
}
