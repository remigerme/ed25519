CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -DED25519_DEBUG -g -fsanitize=address
INCLUDES = -lm -lgmp -Isrc -Isrc/sha3 -Isrc/montgomery

all: keygen sign verify

SHA3_RAW = sha3_utils keccak_rho keccak_c sponge sha3
SHA3_C = $(addsuffix .c,$(SHA3_RAW))
SHA3 = $(addprefix src/sha3/,$(SHA3_C))

keygen: $(SHA3) src/ed25519_utils.c src/ed25519.c src/keygen.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

sign: $(SHA3) src/ed25519_utils.c src/ed25519.c src/sign.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

verify: $(SHA3) src/ed25519_utils.c src/ed25519.c src/verify.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

tests: test-public

test-public: $(SHA3) src/ed25519_utils.c src/ed25519.c tests/public.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f keygen sign verify
