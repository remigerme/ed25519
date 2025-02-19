#!/bin/bash

TESTS=("1" "2" "3" "1024" "sha")

for NAME in "${TESTS[@]}"; do
    FOLDER="tests/test-$NAME"
    PK="$FOLDER/key.bin.pk"
    DATA="$FOLDER/data.bin"
    SIG="$FOLDER/sig.bin"

    OUTPUT=$(./verify $PK $DATA $SIG)
    if [[ $OUTPUT == "ACCEPT" ]]; then
        echo "Verify correct (accept) for test $NAME"
    else
        echo "Verify incorrect (reject) for test $NAME"
        exit 1
    fi
done
