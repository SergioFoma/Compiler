#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dataForASMfromBackend.h"
#include "mathOperatorsInfo.h"
#include "functionsData.h"
#include "paint.h"
#include "myStringFunction.h"

const size_t notUsedLabels = 0;
const size_t stepForLabelsIndex = 2;

informationForASM asmInfo = { };

expertSystemErrors writeASMcommand( tree_t* tree, const char* fileNameForASM ){
    assert( tree );

    FILE* fileForASM = fopen( fileNameForASM, "w" );
    if( fileForASM == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    fprintf( fileForASM, "PUSH 0\n"
                         "POPR RCX\n" );

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

    return notUsedLabels;
}

size_t printNumberInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    fprintf( fileForASM, "PUSH %lg\n", node->data.number );

    return notUsedLabels;
}

size_t printVariableInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    fprintf( fileForASM, "PUSHR RCX\n"
                         "PUSH %lu\n"
                         "ADD\n"
                         "POPR RAX\n"
                         "PUSHM [RAX]\n",
                node->data.variableIndexInArray );

    return notUsedLabels;
}

size_t printMathInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "%s\n", getStringOfMathOperator( node ) );

    return notUsedLabels;
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

    return notUsedLabels;
}

size_t translateAssignmentInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( node->left->nodeValueType != VARIABLE ){
        return notUsedLabels;
    }

    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "PUSHR RCX\n"
                         "PUSH %lu\n"
                         "ADD\n"
                         "POPR RBX\n"
                         "POPM [RBX]\n",
            node->left->data.variableIndexInArray );

    return notUsedLabels;
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

    return notUsedLabels;
}

size_t translateIfInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    size_t labelIndexFromExpression = writeASMcommandFromNode( node->left, fileForASM );
    printElseForIfInASM( node->right, fileForASM, labelIndexFromExpression );

    return asmInfo.currentLabelsIndex;
}

size_t printElseForIfInASM( const node_t* node, FILE* fileForASM, size_t labelIndexFromExpression ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    ++asmInfo.currentLabelsIndex;
    size_t labelIndexForOutFromIf = asmInfo.currentLabelsIndex;

    fprintf( fileForASM, "JMP :%lu\n", labelIndexForOutFromIf );
    fprintf( fileForASM, ":%lu\n", labelIndexFromExpression );

    asmInfo.currentLabelsIndex += stepForLabelsIndex;

    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }
    fprintf( fileForASM, ":%lu\n", labelIndexForOutFromIf );

    return asmInfo.currentLabelsIndex;

}
size_t translateElseInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    size_t labelIndexInIf = asmInfo.currentLabelsIndex + 1;
    fprintf( fileForASM, "JMP :%lu\n", labelIndexInIf );
    fprintf( fileForASM, ":%lu\n", asmInfo.currentLabelsIndex );

    asmInfo.currentLabelsIndex += stepForLabelsIndex;

    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }
    fprintf( fileForASM, ":%lu\n", labelIndexInIf );

    return notUsedLabels;
}

size_t translateWhileInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    ++asmInfo.currentLabelsIndex;
    size_t labelsIndexForStartWhile = asmInfo.currentLabelsIndex;
    fprintf( fileForASM, ":%lu\n", labelsIndexForStartWhile );

    ++asmInfo.currentLabelsIndex;
    size_t labelIndexFromExpression = writeASMcommandFromNode( node->left, fileForASM );
    ++asmInfo.currentLabelsIndex;
    printElseForWhileInASM( node->right, fileForASM, labelsIndexForStartWhile, labelIndexFromExpression );

    return notUsedLabels;
}

size_t printElseForWhileInASM( const node_t* node, FILE* fileForASM, size_t oldLabelsIndex, size_t labelIndexFromExpression ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    fprintf( fileForASM, "JMP :%lu\n", oldLabelsIndex );
    fprintf( fileForASM, ":%lu\n", labelIndexFromExpression );

    return notUsedLabels;
}

size_t translateFunction( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( node->right == NULL ){
        return translateFunctionDeclaration( node->left, fileForASM );
    }

    return translateFunctionDefinition( node, fileForASM );
}

size_t translateFunctionDefinition( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    size_t labelAfterFunction = asmInfo.currentLabelsIndex;
    fprintf( fileForASM, "JMP :%lu\n", labelAfterFunction );
    ++asmInfo.currentLabelsIndex;

    printFunctionParameters( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, ":%lu\n", labelAfterFunction );

    return notUsedLabels;
}

size_t printFunctionParameters( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    printFunctionLabel( node->left, fileForASM );

    fprintf( fileForASM, "PUSHR REX\n"
                         "PUSHR RCX\n"
                         "PUSHR REX\n"
                         "ADD\n"
                         "POPR RCX\n"
                         "POPM [RCX]\n" );    // make new memory area

    if( node->right ){
        printFunctionArgumentsInDefinition( node->right, fileForASM );
    }

    return notUsedLabels;
}

size_t countingVariablesInFunction( const node_t* node ){
    assert( node );

    for( size_t funcIndex = 0; funcIndex < functionInformations.countOfFunction; funcIndex++ ){
        for( size_t varIndex = 0; varIndex < arrayWithSizeOfEveryFunctions[ funcIndex ].freeIndexNow; varIndex++ ){
            if( node == arrayWithInfoForFunctions[ funcIndex ][ varIndex ].nodeAddress ){
                return arrayWithSizeOfEveryFunctions[ funcIndex ].maximumIndexOfVariable;
            }
        }
    }

    return 0;
}

size_t printFunctionArgumentsInDefinition( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( node->right ){
        printFunctionArgumentsInDefinition( node->right, fileForASM );
    }

    fprintf( fileForASM, "PUSHR RCX\n"
                         "PUSH %lu\n"
                         "ADD\n"
                         "POPR RBX\n"
                         "POPM [RBX]\n",
            node->left->data.variableIndexInArray );

    return notUsedLabels;
}

size_t translateFunctionDeclaration( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    size_t countOfVariablesInFunction = countingVariablesInFunction( node->left );
    fprintf( fileForASM, "PUSH %lu\n"
                         "POPR REX\n", countOfVariablesInFunction );

    if( node->right ){
        printFunctionArgumentsInDeclaration( node->right, fileForASM );
    }

    fprintf( fileForASM, "CALL " );

    printFunctionLabel( node->left, fileForASM );
    return notUsedLabels;
}

size_t printFunctionArgumentsInDeclaration( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    if( node->right ){
        printFunctionArgumentsInDeclaration( node->right, fileForASM );
    }

    return notUsedLabels;
}

size_t printFunctionLabel( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    char* nameOfFunction = searchNameOfFunction( node );

    size_t indexInLabelsArray = getIndexOfFunctionAndHerLabel( nameOfFunction );

    if( arrayWithFunctionAndLabels[ indexInLabelsArray ].label == -1 ){
        arrayWithFunctionAndLabels[ indexInLabelsArray ].label = asmInfo.currentLabelsIndex;
        ++asmInfo.currentLabelsIndex;
    }

    fprintf( fileForASM, ":%d\n", arrayWithFunctionAndLabels[ indexInLabelsArray ].label );

    return notUsedLabels;
}

char* searchNameOfFunction( const node_t* nodeWithFunction ){
    assert( nodeWithFunction );

    for( size_t functionIndex = 0; functionIndex < functionInformations.countOfFunction; functionIndex++ ){
        for( size_t varIndex = 0; varIndex < arrayWithSizeOfEveryFunctions[ functionIndex ].freeIndexNow; varIndex++ ){
            if( arrayWithInfoForFunctions[ functionIndex ][ varIndex ].nodeAddress == nodeWithFunction ){
                return arrayWithInfoForFunctions[ functionIndex ][ varIndex ].nameOfVariable;
            }
        }
    }

    return NULL;
}

size_t getIndexOfFunctionAndHerLabel( char* nameOfFunction ){
    assert( nameOfFunction );

    for( size_t labelIndex = 0; functionInformations.countOfFunction - 1; labelIndex++ ){
        if( strcmp( nameOfFunction, arrayWithFunctionAndLabels[ labelIndex ].nameOfFunction ) == 0 ){
            return labelIndex;
        }
    }

    return 0;
}

size_t translateReturnInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );

    fprintf( fileForASM, "PUSHR RCX\n"
                         "PUSHM [RCX]\n"
                         "SUB\n"
                         "POPR RCX\n"
                         "RET\n" );       // return to the previous memory area

    return notUsedLabels;
}

size_t translatePrintInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    if( !( node->nodeValueType == STATEMENT && ( node->data.statement == OPERATOR_END || node->data.statement == PRINT ) ) ){
        writeASMcommandFromNode( node, fileForASM );
        fprintf( fileForASM, "OUT\n" );
        return notUsedLabels;
    }

    if( node->left ){
        translatePrintInASM( node->left, fileForASM );
    }
    if( node->right ){
        translatePrintInASM( node->right, fileForASM );
    }

    return notUsedLabels;
}

size_t translateInputInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    fprintf( fileForASM, "IN\n"
                         "PUSHR RDX\n" );

    return notUsedLabels;
}

size_t printExpressionInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( node->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){
            return arrayWithExpressions[ expressionIndex ].translateExpressionInASMcommand( node, fileForASM );
        }
    }

    return notUsedLabels;
}

size_t translateAboveOrEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JAE :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t translateBelowOrEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JBE :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t translateEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JE :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t translateNoteEqualInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JNE :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t translateBelowInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JB :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t translateAboveInASM( const node_t* node, FILE* fileForASM ){
    assert( node );
    assert( fileForASM );

    writeASMcommandFromNode( node->left, fileForASM );
    writeASMcommandFromNode( node->right, fileForASM );

    fprintf( fileForASM, "JA :%lu\n", asmInfo.currentLabelsIndex );

    return asmInfo.currentLabelsIndex;
}

size_t notTranslateInASM( const node_t* node, FILE* fileForASM ){
    return notUsedLabels;
}

