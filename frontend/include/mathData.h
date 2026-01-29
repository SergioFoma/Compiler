#ifndef H_MATH_DATA
#define H_MATH_DATA

#include "tree.h"

enum mathErrors {
    CORRECT_CALCULATE           = 0,
    CORRECT_DIFFERENTIATION     = 1,
    NULL_PTR                    = 2
};

enum statusOfFind {
    DETECTED_VAR        = 0,
    NOT_DETECTED_VAR    = 1,
    ERROR_OF_FIND_VAR   = 2
};

node_t* copyNode( node_t* node );

node_t* makeConstNode( double value );

node_t* newMathNode( typeOfDataInNode nodeType, typeOfMathOperation mathOperator, node_t* leftNode, node_t* rightNode );

void destroyArrayWithVariables( );

void printArrayWithVariablesInFile( const char* fileNameForVarDump );

#endif
