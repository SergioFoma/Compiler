#!/bin/bash

BUILD_DIRS=( "frontend" )

OUT_PATTERN="*.out"

MAKE_COMMAND="make"

echo "_________________STARTS OF MANUFACTURING___________________"

for dir in "${BUILD_DIRS[@]}"; do
    if  [ -f "$dir/Makefile" ]; then
        ( cd "$dir" && $MAKE_COMMAND )
    fi
done

echo "_______________STARTS Of PROGRAM_____________________"

for out in $(find . -name "$OUT_PATTERN" -type f ); do
    "$out"
done

