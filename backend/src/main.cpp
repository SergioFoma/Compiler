#include <stdio.h>

#include "dataForASMfromBackend.h"
#include "functionsData.h"
#include "paint.h"
#include "tree.h"
#include "parseFileDataBase.h"

int main(){

    tree_t treeFromAST = {};

    initFunctionsData( countOfFunctionsForInitialization, functionInformations.capacityOfFuncArray );

    expertSystemErrors statusOfCreate = createTreeFromFile( &treeFromAST );

    if( statusOfCreate != CORRECT_WORK ){
        colorPrintf( NOMODE, RED, "Error of create tree from AST in backend:%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    makeArrayWithFunctionAndLabels();

    writeASMcommand( &treeFromAST );

    dumpFunctionArrayInFile();

    destroyFunctionsData();

    destroyArrayWithFunctionAndLabels();

    destroyTree( &treeFromAST );
}
