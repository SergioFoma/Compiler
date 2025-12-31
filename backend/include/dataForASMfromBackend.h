#ifndef H_DATA_FOR_ASM_FROM_BACKEND
#define H_DATA_FOR_ASM_FROM_BACKEND

#include <stdio.h>

#include "tree.h"
#include "parseFileDataBase.h"


struct informationForASM {
    size_t currentLabelsIndex;
    size_t indexOfMemoryArea;
    size_t countOfVariables;
};

enum partOfFunction {
    DEFINITION      = 0,
    DECLARATION     = 1
};

expertSystemErrors writeASMcommand( tree_t* tree );

size_t writeASMcommandFromNode( const node_t* node, FILE* fileForASM );

size_t printNumberInASM( const node_t* node, FILE* fileForASM );

size_t printVariableInASM( const node_t* node, FILE* fileForASM );

size_t printMathInASM( const node_t* node, FILE* fileForASM );

size_t printStatementInASM( const node_t* node, FILE* fileForASM );

size_t printExpressionInASM( const node_t* node, FILE* fileForASM );

const char* getStringOfMathOperator( const node_t* node );

size_t translateAssignmentInASM( const node_t* node, FILE* fileForASM );

size_t translateEndOperatorInASM( const node_t* node, FILE* fileForASM );

size_t translateIfInASM( const node_t* node, FILE* fileForASM );

size_t printElseForIfInASM( const node_t* node, FILE* fileForASM, size_t labelIndexForIf );

size_t translateElseInASM( const node_t* node, FILE* fileForASM );

size_t translateWhileInASM( const node_t* node, FILE* fileForASM );

size_t printElseForWhileInASM( const node_t* node, FILE* fileForASM, size_t labelIndexForStartWhile, size_t labelIndexFromExpression );

size_t translateFunction( const node_t* node, FILE* fileForASM );

size_t translateFunctionDefinition( const node_t* node, FILE* fileForASM );

size_t printFunctionParameters( const node_t* node, FILE* fileForASM, partOfFunction functionPart );

size_t printFunctionArguments( const node_t* node, FILE* fileForASM, partOfFunction functionPart );

size_t translateFunctionDeclaration( const node_t* node, FILE* fileForASM );

size_t printFunctionLabel( const node_t* node, FILE* fileForASM );

size_t translateReturnInASM( const node_t* node, FILE* fileForASM );

size_t translatePrintInASM( const node_t* node, FILE* fileForASM );

size_t translateInputInASM( const node_t* node, FILE* fileForASM );

size_t translateAboveOrEqualInASM( const node_t* node, FILE* fileForASM );

size_t translateBelowOrEqualInASM( const node_t* node, FILE* fileForASM );

size_t translateEqualInASM( const node_t* node, FILE* fileForASM );

size_t translateNoteEqualInASM( const node_t* node, FILE* fileForASM );

size_t translateBelowInASM( const node_t* node, FILE* fileForASM );

size_t translateAboveInASM( const node_t* node, FILE* fileForASM );

size_t notTranslateInASM( const node_t* node, FILE* fileForASM );

#endif
