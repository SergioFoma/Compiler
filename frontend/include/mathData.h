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

mathErrors calculateTheFunctionValue( tree_t* tree );

double calculateValue( node_t* node );

double doMathOperations( node_t* node, double firstNumber, double secondNumber );

node_t* copyNode( node_t* node );

node_t* makeConstNode( double value );

node_t* newMathNode( typeOfDataInNode nodeType, typeOfMathOperation mathOperator, node_t* leftNode, node_t* rightNode );

void summaryOfConstants( tree_t* treeForOptimisation );

void optimisationConsts( node_t* node );

bool optimisationFuncWithTwoArg( node_t* node );

bool optimisationFuncWithOneArg( node_t* node );

void removingNeutralElements( tree_t* treeForOptimisation );

void optimisationNeutralElem( node_t* node );

bool mulOnZero( node_t* node );

bool mulOnSingleLeftNode( node_t* node );

bool mulOnSingleRightNode( node_t* node );

bool divideZeroByNum( node_t* node );

bool addWithLeftZeroNode( node_t* node );

bool addWithRightZeroNode( node_t* node );

statusOfFind variableSearching( const node_t* node, size_t variable );

void destroyArrayWithVariables();

void printArrayWithVariablesInFile();

double doAdd( double firstNumber, double secondNumber );
double doSub( double firstNumber, double secondNumber );
double doMul( double firstNumber, double secondNumber );
double doDiv( double firstNumber, double secondNumber );
double doLn( double firstNumber, double secondNumber );
double doPow( double firstNumber, double secondNumber );
double doSin( double firstNumber, double secondNumber );
double doCos( double firstNumber, double secondNumber );
double doTg( double firstNumber, double secondNumber );
double doCtg( double firstNumber, double secondNumber );
double doArcsin( double firstNumber, double secondNumber );
double doArccos( double firstNumber, double secondNumber );
double doArctg( double firstNumber, double secondNumber );
double doArcctg( double firstNumber, double secondNumber );
double doSh( double firstNumber, double secondNumber );
double doCh( double firstNumber, double secondNumber );
double doTh( double firstNumber, double secondNumber );
double doCth( double firstNumber, double secondNumber );
double doExp( double firstNumber, double secondNumber );
double doSqrt( double firstNumber, double secondNumber );

#endif
