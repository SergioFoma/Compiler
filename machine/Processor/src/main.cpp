#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "spuCheckError.h"
#include "stackCheckError.h"
#include "paint.h"
#include "spuLoop.h"
#include "softProcessor.h"

int main(int argc, char** argv){

    Processor SPU = {};

    processorInit( &SPU );

    calculatorErrors typeOfErr = SUCCESSFUL;
    if( argc > 1 && strcmp( argv[1], "--softProcessor" ) == 0 ){
        typeOfErr = calculationFromProcessor( &SPU, argv[2] );
        if( typeOfErr != SUCCESSFUL ){
            colorPrintf( NOMODE, RED, "\n\nMain got an error of type: %d\n\n", typeOfErr );
        }
    }

    processorDestroy( &SPU );

    return 0;
}
