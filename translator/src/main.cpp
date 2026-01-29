#include <stdio.h>
#include <stdlib.h>

#include "translator.h"

int main( int argc, char** argv ){

    if( argc > 2 ){
        translateLanguageOnRussian( argv[1], argv[2] );
    }
    
    return 0;
}
