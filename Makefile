CC = gcc
CFLAGS = -Wall -Wextra -O2 # -Werror v-DED25519_DEBUG -g -fsanitize=address
INCLUDES = -lm -lgmp -Isrc -lssl -lcrypto

all: keygen sign verify

keygen: src/ed25519_utils.c src/ed25519.c src/keygen.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

sign: src/ed25519_utils.c src/ed25519.c src/sign.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

verify: src/ed25519_utils.c src/ed25519.c src/verify.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

tests: test-public all
	./test-public
	./tests/sign.sh
	./tests/verify.sh

test-public: src/ed25519_utils.c src/ed25519.c tests/public.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f keygen sign verify
