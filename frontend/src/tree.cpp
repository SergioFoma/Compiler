#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"


treeErrors initNode( node_t** node, typeOfDataInNode typeOfData, treeElem_t element ){
    assert( node );

    *node = ( node_t* )calloc( oneStruct, sizeof( node_t ) );
    if( *node == NULL ){
        return NOT_ENOUGH_MEMORY;
    }

    initializationDataInNode( node, typeOfData, element );

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->parent = NULL;

    return CORRECT_TREE;
}

treeErrors initTree( tree_t* tree ){
    assert( tree );

    treeElem_t data;
    data.number = maxTreeValue;
    treeErrors errorWithInitialization = initNode( &(tree->rootTree), typeOfRoot, data );

    if( errorWithInitialization != CORRECT_TREE ){
        return errorWithInitialization;
    }

    return CORRECT_TREE;
}

treeErrors initializationDataInNode( node_t** node, typeOfDataInNode typeOfData, treeElem_t element ){
    if( node == NULL ){
        return NODE_NULL_PTR;
    }
    switch( typeOfData ){
        case NUMBER:
            (*node)->nodeValueType = NUMBER;
            (*node)->data.number = element.number;
            break;
        case OPERATOR:
            (*node)->nodeValueType = OPERATOR;
            (*node)->data.mathOperation = element.mathOperation;
            break;
        case VARIABLE:
            (*node)->nodeValueType = VARIABLE;
            (*node)->data.variableIndexInArray = element.variableIndexInArray;
            break;
        case STATEMENT:
            (*node)->nodeValueType = STATEMENT;
            (*node)->data.statement = element.statement;
            break;
        case EXPRESSION:
            (*node)->nodeValueType = EXPRESSION;
            (*node)->data.expressionOperator = element.expressionOperator;
            break;
        default:
            return NOT_CORRECT_TYPE_OF_DATA;
            break;
    }

    return CORRECT_TREE;
}


void destroyNode( node_t* node ){
    if( node == NULL ){
        return ;
    }

    if( node->left  ){
        destroyNode( node->left );
    }

    if( node->right ){
        destroyNode( node->right );
    }

    free( node );
}

void destroyTree( tree_t* tree ){
    if( tree == NULL ){
        return ;
    }

    destroyNode( tree->rootTree );

    *tree = {};
}

node_t* newStatementNode( typeOfDataInNode nodeType, typeOfStatement typeOfStatement, node_t* leftNode, node_t* rightNode ){

    node_t* newStatementNode = NULL;
    treeElem_t data = {};
    data.statement = typeOfStatement;

    initNode( &newStatementNode, nodeType, data );
    newStatementNode->left = leftNode;
    newStatementNode->right = rightNode;

    if( newStatementNode->left ){
        newStatementNode->left->parent = newStatementNode;
    }
    if( newStatementNode->right ){
        newStatementNode->right->parent = newStatementNode;
    }

    return newStatementNode;
}

node_t* newExpressionNode( typeOfDataInNode nodeType, typeOfExpressions typeOfExpression, node_t* leftNode, node_t* rightNode ){

    node_t* newExpressionNode = NULL;
    treeElem_t data = {};
    data.expressionOperator = typeOfExpression;

    initNode( &newExpressionNode, nodeType, data );
    newExpressionNode->left = leftNode;
    newExpressionNode->right = rightNode;

    if( newExpressionNode->left ){
        newExpressionNode->left->parent = newExpressionNode;
    }
    if( newExpressionNode->right ){
        newExpressionNode->right->parent = newExpressionNode;
    }

    return newExpressionNode;
}
