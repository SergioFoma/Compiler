#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "treeDump.h"
#include "parseFileDataBase.h"
#include "dataForBackend.h"
#include "mathData.h"

int main( int argc, char** argv ){

    tree_t tree = {};

    if( argc > 2 ){
        createTreeByRecursiveDescent( &tree, argv[1], argv[2] );
    }

    if( argc > 3 ){
        writeAST( &tree, argv[3] );
    }

    dumpTree( &tree );
    destroyTree( &tree );

    if( argc > 4 ){
        printArrayWithVariablesInFile( argv[4] );
    }
    destroyArrayWithVariables();

    return 0;
}



