# ed25519

The code have been implemented using [RFC 8032](https://datatracker.ietf.org/doc/html/rfc8032).

## Installation

```shell
make
```

This builds three targets : `keygen`, `sign` and `verify`.

## Tests

The following command will compile and run tests automatically :

```shell
make tests
```

Information about performed tests will be displayed. If a test fails it returns an error code, otherwise it returns with code 0 if successfull.

All tests given in the RFC are implemented.

Code implemented for the "Advanced Cryptology" course at École polytechnique by Rémi Germe.
