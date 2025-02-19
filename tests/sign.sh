#!/bin/bash

TESTS=("1" "2" "3" "1024" "sha")

for NAME in "${TESTS[@]}"; do
    FOLDER="tests/test-$NAME"
    PREFIX="$FOLDER/key.bin"
    DATA="$FOLDER/data.bin"
    SIG="$FOLDER/computed-sig"
    EXPECTED_SIG="$FOLDER/sig.bin"

    ./sign $PREFIX $DATA $SIG
    if cmp -s $SIG $EXPECTED_SIG; then
        echo "Signature correct for test $NAME"
    else
        echo "Signature incorrect for test $NAME"
        rm $SIG
        exit 1
    fi
    rm $SIG
done
