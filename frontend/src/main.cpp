#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "treeDump.h"
#include "parseFileDataBase.h"
#include "mathematicalCalculator.h"
#include "dataForBackend.h"

int main(){

    tree_t tree = {};

    createTreeByRecursiveDescent( &tree );

    writeAST( &tree );

    dumpTree( &tree );
    destroyTree( &tree );

    //printArrayWithVariablesInFile();
    destroyArrayWithVariables();

    return 0;
}



