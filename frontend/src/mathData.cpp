#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "mathData.h"
#include "paint.h"
#include "myStringFunction.h"
#include "mathOperatorsInfo.h"

const double epsilon = 1e-5;

#define copyR( node ) copyNode( node->right )
#define copyL( node ) copyNode( node->left )

#define deleteNode( node )              \
    destroyNode( node );                \
    node = NULL;                        \

#define deleteL( node ) deleteNode( node->left )
#define deleteR( node ) deleteNode( node->right)


node_t* copyNode( node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    node_t* newMathNode = NULL;
    initNode( &newMathNode, node->nodeValueType, node->data );

    newMathNode->left = copyL( node );
    newMathNode->right = copyR( node );

    if( newMathNode->left ){
        newMathNode->left->parent = newMathNode;
    }
    if( newMathNode->right ){
        newMathNode->right->parent = newMathNode;
    }

    return newMathNode;
}

node_t* makeConstNode( double value ){
    node_t* newMathNode = NULL;
    treeElem_t data = {};
    data.number = value;
    initNode( &newMathNode, NUMBER, data );
    return newMathNode;
}

node_t* newMathNode( typeOfDataInNode nodeType, typeOfMathOperation mathOperator, node_t* leftNode, node_t* rightNode ){

    node_t* newMathNode = NULL;
    treeElem_t data = {};
    data.mathOperation = mathOperator;

    initNode( &newMathNode, nodeType, data );
    newMathNode->left = leftNode;
    newMathNode->right = rightNode;
    if( newMathNode->left ){
        newMathNode->left->parent = newMathNode;
    }
    if( newMathNode->right ){
        newMathNode->right->parent = newMathNode;
    }

    return newMathNode;
}

void destroyArrayWithVariables(){
    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        free( arrayWithVariables[ varIndex ].nameOfVariable );
    }

    free( arrayWithVariables );
    free( arrayWithVariableValue );
}

void printArrayWithVariablesInFile( const char* fileNameForVarDump ){
    assert( fileNameForVarDump );

    FILE* fileForVar = fopen( fileNameForVarDump, "w" );
    fprintf( fileForVar, "array with variables[ %lu ] = {\n", infoForVarArray.freeIndexNow );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        fprintf( fileForVar, "\t{ %s  , %lu  },\n", arrayWithVariables[ varIndex ].nameOfVariable, arrayWithVariables[ varIndex ].variableIndexInArray );
    }
    fprintf( fileForVar, "};\ncapacity = %lu\n\n", infoForVarArray.capacity );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        fprintf( fileForVar, "var[%lu] = %lg\n", varIndex, arrayWithVariableValue[ varIndex ] );
    }

    fclose( fileForVar );
}
