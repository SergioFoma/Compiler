#ifndef H_DATA_FOR_ASM_FROM_LANGUAGE
#define H_DATA_FOR_ASM_FROM_LANGUAGE

#include "tree.h"
#include "parseFileDataBase.h"

writeAST( tree_t* tree );

expertSystemErrors writeCommandForAssemble( );

void writeCommandForASM( char** ptrOnSymbolInPosition, FILE* fileForASM );


#endif
