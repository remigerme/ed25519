#include "keccak_rho.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "sha3_utils.h"

/**
 * Index of the lane (x, y) of the state
 */
int idx(int x, int y) { return 5 * x + y; }

const size_t RHO_OFFSETS[5][5] = {
    {0, 36, 3, 105 % 64, 210 % 64},
    {1, 300 % 64, 10, 45, 66 % 64},
    {190 % 64, 6, 171 % 64, 15, 253 % 64},
    {28, 55, 153 % 64, 21, 120 % 64},
    {91 % 64, 276 % 64, 231 % 64, 136 % 64, 78 % 64}};

const uint64_t RC_CST[24] = {0x1,
                             0x8082,
                             0x800000000000808A,
                             0x8000000080008000,
                             0x808B,
                             0x80000001,
                             0x8000000080008081,
                             0x8000000000008009,
                             0x8A,
                             0x88,
                             0x80008009,
                             0x8000000A,
                             0x8000808B,
                             0x800000000000008B,
                             0x8000000000008089,
                             0x8000000000008003,
                             0x8000000000008002,
                             0x8000000000000080,
                             0x800A,
                             0x800000008000000A,
                             0x8000000080008081,
                             0x8000000000008080,
                             0x80000001,
                             0x8000000080008008};

void print_state(uint64_t *A) {
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x)
            printf("%016" PRIX64 " ", A[idx(x, y)]);
        printf("\n");
    }
    printf("\n");
}

/**
 * See section 3.1.2 p9.
 * @param in Array of size 200 bytes (=1600 bits) to convert into a state
 * @param A Destination state modified by the function
 */
void state_from_string(char *in, uint64_t *A) {
    for (int x = 0; x < 5; ++x)
        for (int y = 0; y < 5; ++y)
            A[idx(x, y)] = u64_from_chars(&in[8 * (5 * y + x)]);
}

/**
 * See section 3.1.3 p10.
 * @param A State to convert into a string
 * @param out Destination array of size 200 bytes (=1600 bits) modified
 */
void state_to_string(uint64_t *A, char *out) {
    for (int x = 0; x < 5; ++x)
        for (int y = 0; y < 5; ++y)
            u64_to_chars(A[idx(x, y)], &out[8 * (5 * y + x)]);
}

/**
 * See section 3.2.1 p12.
 * @param A State modified in place by theta procedure
 */
void theta(uint64_t *A) {
    // Step 1
    uint64_t C[5];
    for (int x = 0; x < 5; ++x)
        C[x] = A[idx(x, 0)] ^ A[idx(x, 1)] ^ A[idx(x, 2)] ^ A[idx(x, 3)] ^
               A[idx(x, 4)];

    // Step 2
    uint64_t D[5];
    for (int x = 0; x < 5; ++x)
        D[x] = C[((x - 1) % 5 + 5) % 5] ^ rotl_64(C[(x + 1) % 5], 1);

    // Step 3
    for (int x = 0; x < 5; ++x)
        for (int y = 0; y < 5; ++y)
            A[idx(x, y)] ^= D[x];
}

/**
 * See section 3.2.1 p12.
 * @param A State modified in place by rho procedure
 */
void rho(uint64_t *A) {
    uint64_t A_[25];
    // Step 1
    A_[idx(0, 0)] = A[idx(0, 0)];
    // Step 2
    int x = 1;
    int y = 0;
    // Step 3
    for (int t = 0; t < 24; ++t) {
        A_[idx(x, y)] = rotl_64(A[idx(x, y)], RHO_OFFSETS[x][y]);
        int temp = y;
        y = (2 * x + 3 * y) % 5;
        x = temp;
    }
    // Step 4
    memcpy(A, A_, 25 * 8);
}

/**
 * See section 3.2.3 p14.
 * @param A State modified in place by pi procedure
 */
void pi(uint64_t *A) {
    uint64_t A_temp[25];
    for (int x = 0; x < 5; ++x)
        for (int y = 0; y < 5; ++y)
            A_temp[idx(x, y)] = A[idx((x + 3 * y) % 5, x)];

    memcpy(A, A_temp, 25 * 8);
}

/**
 * See section 3.2.4 p15.
 * @param A State modified in place by chi procedure
 */
void chi(uint64_t *A) {
    uint64_t A_temp[25];
    for (int x = 0; x < 5; ++x)
        for (int y = 0; y < 5; ++y)
            A_temp[idx(x, y)] =
                A[idx(x, y)] ^
                ((A[idx((x + 1) % 5, y)] ^ (~0)) & A[idx((x + 2) % 5, y)]);

    memcpy(A, A_temp, 25 * 8);
}

char rc(int t) {
    if (t % 255 == 0)
        return 1;

    char R = 1;
    for (int i = 1; i <= t % 255; ++i) {
        char r8 = get_bit(R, 7);
        R <<= 1;
        set_bit(&R, 0, get_bit(R, 0) ^ r8);
        set_bit(&R, 4, get_bit(R, 4) ^ r8);
        set_bit(&R, 5, get_bit(R, 5) ^ r8);
        set_bit(&R, 6, get_bit(R, 6) ^ r8);
    }
    return get_bit(R, 0);
}

void iota(uint64_t *A, int i) {
    /*
    The following code was used to precompute constants.

    char RC[8];
    for (int b = 0; b < 8; ++b)
        RC[b] = 0;

    for (int j = 0; j <= 6; ++j) {
        int b = (1 << j) - 1;
        set_bit(&RC[b / 8], b % 8, rc(j + 7 * i));
    }

    printf("%" PRIX64 "\n", u64_from_chars(RC));
    */
    A[idx(0, 0)] ^= RC_CST[i];
}

/**
 * See section 3.3 p16.
 * @param A State modified in place during the round
 * @param i Index of the round (for iota procedure)
 */
void rnd(uint64_t *A, int i) {
    theta(A);
#if DEBUG
    printf("After theta :\n");
    print_state(A);
#endif

    rho(A);
#if DEBUG
    printf("After rho :\n");
    print_state(A);
#endif

    pi(A);
#if DEBUG
    printf("After pi :\n");
    print_state(A);
#endif

    chi(A);
#if DEBUG
    printf("After chi :\n");
    print_state(A);
#endif

    iota(A, i);
#if DEBUG
    printf("After iota :\n");
    print_state(A);
#endif
}

void keccak_rho(size_t nr, char *in, char *out) {
    // Step 1
    uint64_t A[25];
    state_from_string(in, A);

#if DEBUG
    printf("Before rounds :\n");
    print_state(A);
#endif

    // Step 2
    for (size_t i = 0; i < nr; ++i)
        rnd(A, i);

#if DEBUG
    printf("After rounds :\n");
    print_state(A);
#endif

    // Step 3 and 4
    state_to_string(A, out);
}
