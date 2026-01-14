#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "paint.h"
#include "parseFileDataBase.h"
#include "mathematicalCalculator.h"
#include "dataForBackend.h"

int main(){

    tree_t treeFromAST = {};

    expertSystemErrors statusOfCreate = createTreeFromFile( &treeFromAST );

    if( statusOfCreate != CORRECT_WORK ){
        colorPrintf( NOMODE, RED, "Error of create tree from AST in middleend:%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    summaryOfConstants( &treeFromAST );

    writeAST( &treeFromAST );

    destroyTree( &treeFromAST );

    return 0;
}




