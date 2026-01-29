#include <stdio.h>

#include "dataForASMfromBackend.h"
#include "functionsData.h"
#include "paint.h"
#include "tree.h"
#include "parseFileDataBase.h"

int main( int argc, char** argv ){

    tree_t treeFromAST = {};

    initFunctionsData( countOfFunctionsForInitialization, functionInformations.capacityOfFuncArray );
    expertSystemErrors statusOfCreate = CORRECT_WORK;

    if( argc > 1 ){
        statusOfCreate = createTreeFromFile( &treeFromAST, argv[1] );
    }

    if( statusOfCreate != CORRECT_WORK ){
        colorPrintf( NOMODE, RED, "Error of create tree from AST in backend:%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    makeArrayWithFunctionAndLabels();

    if( argc > 2 ){
        writeASMcommand( &treeFromAST, argv[2]);
    }

    if( argc > 3 ){
        dumpFunctionArrayInFile( argv[3] );
    }

    destroyFunctionsData();

    destroyArrayWithFunctionAndLabels();

    destroyTree( &treeFromAST );
}
