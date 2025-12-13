#ifndef H_DATA_FOR_ASM_FROM_BACKEND
#define H_DATA_FOR_ASM_FROM_BACKEND

#include <stdio.h>

#include "tree.h"
#include "parseFileDataBase.h"

expertSystemErrors writeASMcommand( tree_t* tree );

void writeASMcommandFromNode( const node_t* node, FILE* fileForASM );

void writeCommand( const node_t* node, FILE* fileForASM );

void printNumberInASM( const node_t* node, FILE* fileForASM );

void printMathInASM( const node_t* node, FILE* fileForASM );

void printStatementInASM( const node_t* node, FILE* fileForASM );

const char* getStringOfMathOperator( const node_t* node );

void translateAssignmentInASM( const node_t* node, FILE* fileForASM );

#endif
