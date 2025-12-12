#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dataForASMfromBackend.h"
#include "mathOperatorsInfo.h"
#include "paint.h"
#include "myStringFunction.h"

expertSystemErrors writeASMcommand( tree_t* tree ){
    assert( tree );

    colorPrintf( NOMODE, YELLOW, "\nEnter the name of file were do you want save ASSEMBLE code: " );

    char* fileNameForWrite = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &fileNameForWrite, &sizeOfAllocationMemory, stdin );
    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileForTree = fopen( fileNameForWrite, "w" );
    if( fileForTree == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    writeASMcommandFromNode( tree->rootTree, fileForTree );

    free( fileNameForWrite );
    fclose( fileForTree );
    return CORRECT_WORK;
}

void writeASMcommandFromNode( node_t* node, FILE* fileForTree ){
    assert( node );

    if( node->left ){
        writeASMcommandFromNode( node->left, fileForTree );
    }
    if( node->right ){
        writeASMcommandFromNode( node->right, fileForTree );
    }

    writeCommand( node, fileForTree );
}

void writeCommand( node_t* node, FILE* fileForTree ){
    assert( node );

    switch( node->nodeValueType ){
        case NUMBER:
            printNumberInASM( node, fileForTree );
            break;
        case OPERATOR:
            printMathInASM( node, fileForTree );
            break;
        default:
            break;
    }
}

void printNumberInASM( node_t* node, FILE* fileForTree ){
    assert( node );

    fprintf( fileForTree, "PUSH %lg\n", node->data.number );
}

void printMathInASM( node_t* node, FILE* fileForTree ){
    assert( node );

    fprintf( fileForTree, "%s\n", getStringOfMathOperator( node ) );
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
