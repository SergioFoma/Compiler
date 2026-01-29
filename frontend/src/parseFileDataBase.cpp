#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "parseFileDataBase.h"
#include "myStringFunction.h"
#include "paint.h"
#include "mathOperatorsInfo.h"
#include "mathData.h"
#include "lexer.h"
#include "fileBufferInfo.h"

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree ){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will find mathematical statement: " );

    FILE* fileForMathStatement = NULL;

    bufferInformation dataBaseFromFile = getBufferFromFile( &fileForMathStatement );
    if( dataBaseFromFile.typeOfErr != correct ){
        return ERROR_WITH_FILE;
    }
    char* ptrOnSymbolInPosition = dataBaseFromFile.buffer;

    infoForCreateTree infoForTree = {};
    initializationTokens( &infoForTree );

    lexAnalysis( &ptrOnSymbolInPosition, &infoForTree );
    dumpLexArrayInFile( &infoForTree );
    infoForTree.currentIndex = 0;                               // start position - zero index

    tree->rootTree = getGeneral( &infoForTree );
    colorPrintf( NOMODE, GREEN, "\nSuccessfully reading an expression from a file\n");


    destroyLexArray( &infoForTree );
    destroyBufferInformation( &dataBaseFromFile );
    fclose( fileForMathStatement );
    return CORRECT_WORK;
}

node_t* getGeneral( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = makeTreeFromOperators( infoForTree );

    printf( "token stoped = %lu\nsize of array = %lu", infoForTree->currentIndex, infoForTree->countOfTokens );
    return nodeOperator;
}

node_t* makeTreeFromOperators( infoForCreateTree* infoForTree ){
    if( infoForTree->currentIndex >= ( infoForTree->freeIndexNow - 1 ) ){
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    if( nodeOperator->right == NULL ){
        nodeOperator->right = makeTreeFromOperators( infoForTree );
    }
    else{
        nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, NULL );
        nodeOperator->right = makeTreeFromOperators( infoForTree );
    }

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}

node_t* getOperator( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, NULL, NULL );

    if( nodeOperator->left = getAssignment( infoForTree )  ){}
    else if( nodeOperator->left = getCondition( infoForTree ) ){}
    else if( nodeOperator->left = getCycle( infoForTree ) ){}
    else if( nodeOperator->left = getPrint( infoForTree ) ){}
    else if( nodeOperator->left = getInput( infoForTree ) ){}
    else if( nodeOperator->left = getFunctionDefinition( infoForTree ) ){}
    else if( nodeOperator->left = getFunctionDeclaration( infoForTree ) ){}
    else if( nodeOperator->left = getReturn( infoForTree ) ){}
    else if( nodeOperator->left = getExpression( infoForTree ) ){}
    else{

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

        destroyNode( nodeOperator );
        nodeOperator = getNodeFromOperatorThatCalledOperator( infoForTree );
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == OPERATOR_END ){
        ++( infoForTree->currentIndex );
    }

    return nodeOperator;
}

node_t* getNodeFromOperatorThatCalledOperator( infoForCreateTree* infoForTree ){
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    nodeOperator->right = getNodeFromOperatorThatCalledOperator( infoForTree );

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}

node_t* getCondition( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == IF &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == IF ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }

            node_t* left = getLogicalOr( infoForTree );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }

            node_t* right = getElse( infoForTree );
            return newStatementNode( STATEMENT, IF, left, right );
          }
    }

    return NULL;
}

node_t* getElse( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getOperator( infoForTree );
    node_t* right = NULL;

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == ELSE ){

        ++( infoForTree->currentIndex );

        right = getOperator( infoForTree );
    }

    return newStatementNode( STATEMENT, ELSE, left, right );

}

node_t* getCycle( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == WHILE &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == WHILE ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }

            node_t* left = getLogicalOr( infoForTree );

            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }

            node_t* right = getElse( infoForTree );
            return newStatementNode( STATEMENT, WHILE, left, right );
          }
    }

    return NULL;

}

node_t* getAssignment( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = NULL;
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == VARIABLE ){
        left = getVariable( infoForTree );
    }
    else{
        return NULL;
    }

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == ASSIGNMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == ASSIGNMENT ){

            ++( infoForTree->currentIndex );

            node_t* right = getLogicalOr( infoForTree );

            return newStatementNode( STATEMENT, ASSIGNMENT, left, right );
        }
    }

    destroyNode( left );
    --( infoForTree->currentIndex );
    return NULL;
}

node_t* getFunctionDefinition( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == DEF ) ){

            return NULL;
    }

    ++( infoForTree->currentIndex );

    node_t* nameOfFunction = getVariable( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

    node_t* functionArgument = getArgumentInFunction( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
        ++( infoForTree->currentIndex );
    }

    node_t* left = newStatementNode( STATEMENT, OPERATOR_END, nameOfFunction, functionArgument );
    node_t* right = getOperator( infoForTree );

    return newStatementNode( STATEMENT, FUNC, left, right );
}

node_t* getFunctionDeclaration( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == VARIABLE ) ){
        return NULL;
    }

    node_t* nameOfFunction = getVariable( infoForTree );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ) ){

        destroyNode( nameOfFunction );
        --( infoForTree->currentIndex );
        return NULL;
    }

    ++( infoForTree->currentIndex );                                                             // skipping the opening par
    node_t* functionArgument = getArgumentInFunction( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
        ++( infoForTree->currentIndex );
    }

    node_t* left = newStatementNode( STATEMENT, OPERATOR_END, nameOfFunction, functionArgument );
    return newStatementNode( STATEMENT, FUNC, left, NULL );
}

node_t* getReturn( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == RET ) ){

        return NULL;
    }

    ++( infoForTree->currentIndex );

    node_t* left = getExpression( infoForTree );

    return newStatementNode( STATEMENT, RET, left, NULL );
}

node_t* getPrint( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* printNode = NULL;

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PRINT ){

        ++( infoForTree->currentIndex );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

        printNode = getArgumentInFunction( infoForTree );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }

        printNode = newStatementNode( STATEMENT, PRINT, printNode, NULL );
    }

    return printNode;
}

node_t* getArgumentInFunction( infoForCreateTree* infoForTree ){
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    nodeOperator->right = getArgumentInFunction( infoForTree );

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}


node_t* getInput( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == INPUT ) {
        ++( infoForTree->currentIndex );
    }
    else{
        return NULL;
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ) {
        ++( infoForTree->currentIndex );
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
    }

    return newStatementNode( STATEMENT, INPUT, NULL, NULL );
}

const char* getEndOfAssignment(){
    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == OPERATOR_END ){
            return arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        }
    }

    return NULL;
}

node_t* getLogicalOr( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getLogicalAnd( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == OR ) ){

        ++( infoForTree->currentIndex );

        node_t* right = getLogicalAnd( infoForTree );

        left = newExpressionNode( EXPRESSION, OR, left, right );
    }

    return left;
}

node_t* getLogicalAnd( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getTypeOfEqual( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == AND ) ){

        ++( infoForTree->currentIndex );

        node_t* right = getTypeOfEqual( infoForTree );

        left = newExpressionNode( EXPRESSION, AND, left, right );
    }

    return left;
}

node_t* getTypeOfEqual( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getCompareOperations( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == EQUAL ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == NOT_EQUAL ) ) {

        typeOfExpressions currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;
        ++( infoForTree->currentIndex );

        node_t* right = getCompareOperations( infoForTree );

        left = newExpressionNode( EXPRESSION, currentExpression, left, right );
    }

    return left;
}

node_t* getCompareOperations( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getExpression( infoForTree );
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType != EXPRESSION ){
        return left;
    }

    typeOfExpressions currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( currentExpression == BELOW || currentExpression == BELOW_OR_EQUAL || currentExpression == ABOVE || currentExpression == ABOVE_OR_EQUAL ) ) {

        ++( infoForTree->currentIndex );

        node_t* right = getExpression( infoForTree );

        left = newExpressionNode( EXPRESSION, currentExpression, left, right );
        currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;
    }

    return left;
}


node_t* getExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getTerm( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == ADD ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == SUB ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getTerm( infoForTree );

        if( currentMathOp == ADD ){
            left = newMathNode( OPERATOR, ADD, left, right );
        }
        else{
            left = newMathNode( OPERATOR, SUB, left, right );
        }
    }

    return left;
}

node_t* getTerm( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getPow( infoForTree );


    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == MUL ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == DIV ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getPow( infoForTree );

        if( currentMathOp == MUL ){
            left = newMathNode( OPERATOR, MUL, left, right );
        }
        else if( currentMathOp == DIV ){
            left = newMathNode( OPERATOR, DIV, left, right );
        }
    }

    return left;
}

node_t* getPow( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getPrimaryExpression( infoForTree );


    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == POW ){

        ++( infoForTree->currentIndex );

        node_t* right = getPrimaryExpression( infoForTree );

        left = newMathNode( OPERATOR, POW, left, right );
    }

    return left;

}

node_t* getPrimaryExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
        ++( infoForTree->currentIndex );

        node_t* nodeFromExpression = getLogicalOr( infoForTree );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }
        return nodeFromExpression;
    }

    node_t* currentNode = ( infoForTree->tokens )[ infoForTree->currentIndex ];
    node_t* resultNode = NULL;

    if( resultNode = getInput( infoForTree ) ){}
    else if( resultNode = getFunctionDeclaration( infoForTree ) ){}
    else if( currentNode->nodeValueType != NUMBER ){
        return getVariable( infoForTree );
    }
    else{
        return getNumber( infoForTree );
    }

    return resultNode;
}

node_t* getFunctionWithOneArgument( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }
            node_t* funcNode = getExpression( infoForTree );

            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }
            return newMathNode( OPERATOR, arrayWithMathInfo[ indexOfMathFunc ].mathOperation, NULL, funcNode );
        }
    }

    return NULL;
}

node_t* getVariable( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    bool isFunction = checkingOnFunction( infoForTree );
    if( isFunction ){
        return getFunctionWithOneArgument( infoForTree );
    }

    bool isStatement = checkingOnStatement( infoForTree );
    if( isStatement ){
        return NULL;
    }

    bool isExpression = checkingOnExpression( infoForTree );
    if( isExpression ){
        return getLogicalOr( infoForTree );
    }

    size_t treeIndex = ( infoForTree->currentIndex );
    ++( infoForTree->currentIndex );
    return copyNode( ( infoForTree->tokens )[ treeIndex ] );
}

bool checkingOnFunction( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){
            return true;
        }
    }

    return false;
}

bool checkingOnStatement( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == arrayWithStatements[ statementIndex ].statement ){

                return true;
            }
    }

    return false;
}

bool checkingOnExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){

                return true;
            }
    }

    return false;
}

node_t* getNumber( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType == NUMBER ){
        size_t treeIndex = infoForTree->currentIndex;
        ++( infoForTree->currentIndex );
        return copyNode( (infoForTree->tokens)[ treeIndex ] );
    }

    colorPrintf( NOMODE, RED, "ERROR:%s %s %d\n typeOfNode = %d, current index = %lu\n", __FILE__, __func__, __LINE__,
                (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType, infoForTree->currentIndex );
    return NULL;
}
