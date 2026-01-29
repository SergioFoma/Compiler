#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "paint.h"
#include "parseFileDataBase.h"
#include "mathematicalCalculator.h"
#include "dataForBackend.h"

int main( int argc, char** argv ){

    tree_t treeFromAST = {};
    expertSystemErrors statusOfCreate = CORRECT_WORK;

    if( argc > 1 ){
        statusOfCreate = createTreeFromFile( &treeFromAST, argv[1] );
    }

    if( statusOfCreate != CORRECT_WORK ){
        colorPrintf( NOMODE, RED, "Error of create tree from AST in middleend:%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    summaryOfConstants( &treeFromAST );

    if( argc > 1 ){
        writeAST( &treeFromAST, argv[1] );
    }

    destroyTree( &treeFromAST );

    return 0;
}




