#ifndef H_DATA_FOR_ASM_FROM_BACKEND
#define H_DATA_FOR_ASM_FROM_BACKEND

#include <stdio.h>

#include "tree.h"
#include "parseFileDataBase.h"

expertSystemErrors writeASMcommand( tree_t* tree );

void writeASMcommandFromNode( node_t* node, FILE* fileForTree );

void writeCommand( node_t* node, FILE* fileForTree );

void printNumberInASM( node_t* node, FILE* fileForTree );

void printMathInASM( node_t* node, FILE* fileForTree );

const char* getStringOfMathOperator( const node_t* node );

#endif
