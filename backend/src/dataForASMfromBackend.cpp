#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dataForASMfromBackend.h"
#include "mathOperatorsInfo.h"
#include "paint.h"
#include "myStringFunction.h"

#define FILE_NAME_FOR_ASM "commonFiles/assemble.asm"            // hardcoding, because the user should not know about the intermediate files.

expertSystemErrors writeASMcommand( tree_t* tree ){
    assert( tree );

    FILE* fileForASM = fopen( FILE_NAME_FOR_ASM, "w" );
    if( fileForASM == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    writeASMcommandFromNode( tree->rootTree, fileForASM );

    fprintf( fileForASM, "OUT\n" );
    fprintf( fileForASM, "HLT\n" );

    fclose( fileForASM );
    return CORRECT_WORK;
}

void writeASMcommandFromNode( node_t* node, FILE* fileForASM ){
    assert( node );

    if( node->left ){
        writeASMcommandFromNode( node->left, fileForASM );
    }
    if( node->right ){
        writeASMcommandFromNode( node->right, fileForASM );
    }

    writeCommand( node, fileForASM );
}

void writeCommand( node_t* node, FILE* fileForASM ){
    assert( node );

    switch( node->nodeValueType ){
        case NUMBER:
            printNumberInASM( node, fileForASM );
            break;
        case OPERATOR:
            printMathInASM( node, fileForASM );
            break;
        default:
            break;
    }
}

void printNumberInASM( node_t* node, FILE* fileForASM ){
    assert( node );

    fprintf( fileForASM, "PUSH %lg\n", node->data.number );
}

void printMathInASM( node_t* node, FILE* fileForASM ){
    assert( node );

    fprintf( fileForASM, "%s\n", getStringOfMathOperator( node ) );
}

const char* getStringOfMathOperator( const node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    for( size_t mathIndex = 0; mathIndex < sizeOfMathArray; mathIndex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ mathIndex ].mathOperation ){
            return arrayWithMathInfo[ mathIndex ].nameOfMathOperation;
        }
    }

    return NULL;
}
