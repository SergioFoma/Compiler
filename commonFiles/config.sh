#!/bin/bash

BUILD_DIRS=( "backend" )

OUT_PATTERN="*.out"

MAKE_COMMAND="make"

BUILD_STATUS=true

echo "_________________STARTS OF MANUFACTURING___________________"

for dir in "${BUILD_DIRS[@]}"; do
    if  [ -f "$dir/Makefile" ]; then
        ( cd "$dir" && $MAKE_COMMAND )

        if [ $? -ne 0 ]; then
            echo "Errors of Makefile in $dir "
            BUILD_STATUS=false
        fi
    fi
done

if [ "$BUILD_STATUS" = false ]; then
    echo "Build ERROR"
    exit 0
fi


echo "_______________STARTS Of PROGRAM_____________________"

for out in $(find . -name "$OUT_PATTERN" -type f ); do
    "$out"
done

