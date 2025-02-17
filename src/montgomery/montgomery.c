#include "montgomery.h"

#include <stdio.h>
#include <stdlib.h>

void point_init(point *p) { mpz_inits(p->x, p->z, NULL); }

void point_clear(point *p) { mpz_clears(p->x, p->z, NULL); }

void point_normalize(point *p, mpz_t mod) {
    if (mpz_cmp_ui(p->z, 0) == 0)
        return;

    mpz_t inv;
    mpz_init(inv);

    mpz_invert(inv, p->z, mod);
    mpz_mul(p->z, p->z, inv);
    mpz_mod(p->z, p->z, mod);
    mpz_mul(p->x, p->x, inv);
    mpz_mod(p->x, p->x, mod);
    mpz_clear(inv);
}

void point_print(point p) { gmp_printf("%Zd : %Zd\n", p.x, p.z); }

void cswap_mpz(int swap, mpz_t x0, mpz_t x1) {
    mpz_t cx0, cx1;
    mpz_init_set(cx0, x0);
    mpz_init_set(cx1, x1);

    mpz_mul_si(x0, x0, 1 - swap);
    mpz_mul_si(x1, x1, swap);
    mpz_add(x0, x0, x1);

    mpz_mul_si(cx0, cx0, swap);
    mpz_mul_si(cx1, cx1, 1 - swap);
    mpz_add(x1, cx0, cx1);

    mpz_clears(cx0, cx1, NULL);
}

void cswap(int swap, point *p, point *q) {
    cswap_mpz(swap, p->x, q->x);
    cswap_mpz(swap, p->z, q->z);
}

void xadd(point *rop, point p, point q, point dif, mpz_t mod) {
    mpz_t u, v, temp;
    mpz_inits(u, v, temp, NULL);

    // Computing u
    mpz_sub(u, p.x, p.z);
    mpz_add(temp, q.x, q.z);
    mpz_mul(u, u, temp);
    mpz_mod(u, u, mod);

    // Computing v
    mpz_add(v, p.x, p.z);
    mpz_sub(temp, q.x, q.z);
    mpz_mul(v, v, temp);
    mpz_mod(v, v, mod);

    // Computing x+
    mpz_add(rop->x, u, v);
    mpz_pow_ui(rop->x, rop->x, 2);
    mpz_mul(rop->x, dif.z, rop->x);
    mpz_mod(rop->x, rop->x, mod);

    // Computing z+
    mpz_sub(rop->z, u, v);
    mpz_pow_ui(rop->z, rop->z, 2);
    mpz_mul(rop->z, dif.x, rop->z);
    mpz_mod(rop->z, rop->z, mod);

    point_normalize(rop, mod);

    mpz_clears(u, v, temp, NULL);
}

void xdbl(point *rop, point p, mpz_t a24, mpz_t mod) {
    mpz_t q, r, s, inv;
    mpz_inits(q, r, s, inv, NULL);

    // Computing q
    mpz_add(q, p.x, p.z);
    mpz_pow_ui(q, q, 2);
    mpz_mod(q, q, mod);

    // Computing r
    mpz_sub(r, p.x, p.z);
    mpz_pow_ui(r, r, 2);
    mpz_mod(r, r, mod);

    // Computing s
    mpz_sub(s, q, r);

    // Setting [2]p.x
    mpz_mul(rop->x, q, r);
    mpz_mod(rop->x, rop->x, mod);

    // Setting [2]p.z, using q as a temp var bc we don't need it anymore
    mpz_mul(q, a24, s);
    mpz_add(q, r, q);
    mpz_mul(rop->z, s, q);
    mpz_mod(rop->z, rop->z, mod);

    point_normalize(rop, mod);

    mpz_clears(q, r, s, inv, NULL);
}

void point_set(point *rop, point p) {
    mpz_set(rop->x, p.x);
    mpz_set(rop->z, p.z);
}

void ladder(mpz_t rop, mpz_t m, mpz_t p, mpz_t a24, mpz_t mod) {
    // Dif
    point u, res;
    point_init(&u);
    mpz_set(u.x, p);
    mpz_set_ui(u.z, 1);

    // res is x0
    point_init(&res);
    mpz_set_ui(res.x, 1);
    mpz_set_ui(res.z, 0);

    point x1;
    point_init(&x1);
    point_set(&x1, u);

    // Temporary point
    point t;
    point_init(&t);

    int n_bits = mpz_sizeinbase(m, 2);
    for (int i = n_bits - 1; i >= 0; --i) {
        int mi = mpz_tstbit(m, i);

        cswap(mi, &res, &x1);

        point_set(&t, res);
        xdbl(&res, res, a24, mod);
        xadd(&x1, t, x1, u, mod);

        cswap(mi, &res, &x1);
    }
    mpz_set(rop, res.x);

    point_clear(&u);
    point_clear(&x1);
    point_clear(&t);
    point_clear(&res);
}

void curve25519_ladder(mpz_t rop, mpz_t m, mpz_t p) {
    mpz_t a24, mod;
    mpz_inits(a24, mod, NULL);

    mpz_set_ui(mod, 1);
    mpz_mul_2exp(mod, mod, 255);
    mpz_sub_ui(mod, mod, 19);

    mpz_set_ui(a24, 121666);

    ladder(rop, m, p, a24, mod);

    mpz_clears(a24, mod, NULL);
}
