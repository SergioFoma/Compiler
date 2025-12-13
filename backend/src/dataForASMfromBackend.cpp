#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dataForASMfromBackend.h"
#include "mathOperatorsInfo.h"
#include "paint.h"
#include "myStringFunction.h"

#define FILE_NAME_FOR_ASM "commonFiles/assemble.asm"            // hardcoding, because the user should not know about the intermediate files.
#define NOT_USED_LABELS 0

size_t currentLabelsIndex = 0;
const size_t stepForLabelsIndex = 2;

expertSystemErrors writeASMcommand( tree_t* tree ){
    assert( tree );

    FILE* fileForASM = fopen( FILE_NAME_FOR_ASM, "w" );
    if( fileForASM == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    writeASMcommandFromNode( tree->rootTree, fileForASM );

    fprintf( fileForASM, "DRAW\n" );
    fprintf( fileForASM, "HLT\n" );

    fclose( fileForASM );
    return CORRECT_WORK;
}

size_t writeASMcommandFromNode( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    switch( node->nodeValueType ){
        case NUMBER:
            return printNumberInASM( node, fileForASM );
            break;
        case OPERATOR:
            return printMathInASM( node, fileForASM );
            break;
        case STATEMENT:
            return printStatementInASM( node, fileForASM );
            break;
        case VARIABLE:
            return printVariableInASM( node, fileForASM );
            break;
        case EXPRESSION:
            return printExpressionInASM( node, fileForASM );
            break;
        default:
            break;
    }
}

size_t printNumberInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    fprintf( fileForASM, "PUSH %lg\n", node->data.number );

    return NOT_USED_LABELS;
}

size_t printVariableInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( node->data.variableIndexInArray == arrayWithVariables[ varIndex ].variableIndexInArray ){
            fprintf( fileForASM, "PUSH %lu\nPOPR RAX\nPUSHM [RAX]\n", node->data.variableIndexInArray );
            return NOT_USED_LABELS;
        }
    }
    return NOT_USED_LABELS;
}

size_t printMathInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "%s\n", getStringOfMathOperator( node ) );

    return NOT_USED_LABELS;
}

const char* getStringOfMathOperator( const node_t* node ){
    assert( node );

    for( size_t mathIndex = 0; mathIndex < sizeOfMathArray; mathIndex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ mathIndex ].mathOperation ){
            return arrayWithMathInfo[ mathIndex ].nameOfMathOperation;
        }
    }

    return NULL;
}

size_t printStatementInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( node->data.statement == arrayWithStatements[ statementIndex ].statement ){
            return arrayWithStatements[ statementIndex ].translateStatementInASMcommand( node, fileForASM );
        }
    }

    return NOT_USED_LABELS;
}

size_t translateAssignmentInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( node->left->nodeValueType != VARIABLE ){
        return NOT_USED_LABELS;
    }

    writeASMcommandFromNode( node->right, fileForASM );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( node->left->data.variableIndexInArray == arrayWithVariables[ varIndex ].variableIndexInArray ){
            fprintf( fileForASM, "PUSH %lu\nPOPR RBX\nPOPM [RBX]\n", node->left->data.variableIndexInArray );
            return NOT_USED_LABELS;
        }
    }

    return NOT_USED_LABELS;
}

size_t translateEndOperatorInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( node->left ){
        writeASMcommandFromNode( node->left, fileForASM );
    }
    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }

    return NOT_USED_LABELS;
}

size_t translateIfInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    size_t labelIndexFromExpression = writeASMcommandFromNode( node->left, fileForASM );
    printElseForIfInASM( node->right, fileForASM, labelIndexFromExpression );

    return currentLabelsIndex;
}

size_t printElseForIfInASM( const node_t* node, FILE* fileForASM, size_t labelIndexFromExpression ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    ++currentLabelsIndex;
    size_t labelIndexForOutFromIf = currentLabelsIndex;

    fprintf( fileForASM, "JMP :%lu\n", labelIndexForOutFromIf );
    fprintf( fileForASM, ":%lu\n", labelIndexFromExpression );

    currentLabelsIndex += stepForLabelsIndex;

    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }
    fprintf( fileForASM, ":%lu\n", labelIndexForOutFromIf );

    return currentLabelsIndex;

}
size_t translateElseInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    size_t labelIndexInIf = currentLabelsIndex + 1;
    fprintf( fileForASM, "JMP :%lu\n", labelIndexInIf );
    fprintf( fileForASM, ":%lu\n", currentLabelsIndex );

    currentLabelsIndex += 2;

    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }
    fprintf( fileForASM, ":%lu\n", labelIndexInIf );

    return NOT_USED_LABELS;
}

size_t translateWhileInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    ++currentLabelsIndex;
    size_t labelsIndexForStartWhile = currentLabelsIndex;
    fprintf( fileForASM, ":%lu\n", labelsIndexForStartWhile );

    ++currentLabelsIndex;
    size_t labelIndexFromExpression = writeASMcommandFromNode( node->left, fileForASM );
    ++currentLabelsIndex;
    printElseForWhileInASM( node->right, fileForASM, labelsIndexForStartWhile, labelIndexFromExpression );

    return NOT_USED_LABELS;
}

size_t printElseForWhileInASM( const node_t* node, FILE* fileForASM, size_t oldLabelsIndex, size_t labelIndexFromExpression ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    fprintf( fileForASM, "JMP :%lu\n", oldLabelsIndex );
    fprintf( fileForASM, ":%lu\n", labelIndexFromExpression );

    return NOT_USED_LABELS;
}

size_t printExpressionInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( node->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){
            return arrayWithExpressions[ expressionIndex ].translateExpressionInASMcommand( node, fileForASM );
        }
    }

    return NOT_USED_LABELS;
}

size_t translateAboveOrEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JAE :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}

size_t translateBelowOrEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JBE :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}

size_t translateEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JE :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}

size_t translateNoteEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JNE :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}

size_t translateBelowInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JB :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}

size_t translateAboveInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JA :%lu\n", currentLabelsIndex );

    return currentLabelsIndex;
}
