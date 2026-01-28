#!/bin/bash

BUILD_DIRS=( "frontend" "middleend" "backend" "machine/ASM" "machine/Processor" "translator")

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
            break
        fi
    fi
done

if [ "$BUILD_STATUS" = false ]; then
    echo "Build ERROR"
    exit 0
fi


echo "_______________FRONTEND STARTED_____________________"
"frontend/Build/frontend.out"

echo "______________MIDDLEEND STARTED_____________"
"middleend/Build/middleend.out"

echo "___________BACKED STARTED________________"
"backend/Build/backend.out"

echo "_____________ASSEMBLER STARTED_____________"
"machine/ASM/Build/ASM.out" "commonFiles/assemble.asm"

#echo "_____________TRANSLATOR STARTED______________"
#"translator/Build/translator.out"
