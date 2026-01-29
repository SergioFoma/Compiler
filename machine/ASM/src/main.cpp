#include <stdio.h>

#include "assembleCode.h"
#include "paint.h"

int main( int argc, char** argv){

    const size_t labelsSize = 30;
    int labels[ labelsSize ] = {};
    typeOfErr codeWithError = OK;

    for( size_t labelsIndex = 0; labelsIndex < labelsSize; labelsIndex++ ){
        labels[ labelsIndex ] = -1;
    }


    if( argc > 2 ){
        codeWithError = assemble( argv[1], argv[2], labels );
        analysisOfAssembleError( codeWithError );
    }
    else{
        colorPrintf( NOMODE, RED, "\nNot founded files for assemble\n");
    }


    if( codeWithError != OK ){
        colorPrintf( NOMODE, RED, "\nError from assemble :%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    return 0;
}
