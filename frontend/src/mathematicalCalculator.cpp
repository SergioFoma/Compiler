#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "mathematicalCalculator.h"
#include "paint.h"
#include "myStringFunction.h"
#include "mathOperatorsInfo.h"
#include "../../commonFiles/config.h"   // first - enter from src, second - enter from fronted

const double epsilon = 1e-5;

#define ADD_( leftNode, rightNode ) newMathNode( OPERATOR, ADD, leftNode, rightNode )
#define SUB_( leftNode, rightNode ) newMathNode( OPERATOR, SUB, leftNode, rightNode )
#define MUL_( leftNode, rightNode ) newMathNode( OPERATOR, MUL, leftNode, rightNode )
#define DIV_( leftNode, rightNode ) newMathNode( OPERATOR, DIV, leftNode, rightNode )
#define COS_( rightNode ) newMathNode( OPERATOR, COS, NULL, rightNode )
#define SIN_( rightNode ) newMathNode( OPERATOR, SIN, NULL, rightNode )
#define LN_( rightNode ) newMathNode( OPERATOR, LN, NULL, rightNode )
#define POW_( leftNode, rightNode ) newMathNode( OPERATOR, POW, leftNode, rightNode )
#define SQRT_( rightNode ) newMathNode( OPERATOR, SQRT, NULL, rightNode )
#define CH_( rightNode ) newMathNode( OPERATOR, CH, NULL, rightNode )
#define SH_( rightNode ) newMathNode( OPERATOR, SH, NULL, rightNode )
#define EXP_( rightNode ) newMathNode( OPERATOR, EXP, NULL, rightNode )

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

statusOfFind variableSearching( const node_t* node, size_t variable ){
    if( node == NULL ){
        return ERROR_OF_FIND_VAR;
    }

    if( node->nodeValueType == VARIABLE && node->data.variableIndexInArray == variable ){
        return DETECTED_VAR;
    }

    statusOfFind leftSearching = variableSearching( node->left, variable );
    statusOfFind rightSearching = variableSearching( node->right, variable );

    if( leftSearching == DETECTED_VAR ){
        return leftSearching;
    }
    if( rightSearching == DETECTED_VAR ){
        return rightSearching;
    }

    return NOT_DETECTED_VAR;
}

void summaryOfConstants( tree_t* treeForOptimisation ){
    assert( treeForOptimisation );

    optimisationConsts( treeForOptimisation->rootTree );

}

void optimisationConsts( node_t* node ){
    assert( node );

    if( node->nodeValueType == NUMBER || node->nodeValueType == VARIABLE ){
        return ;
    }

    if( node->left ){
        optimisationConsts( node->left );
    }
    if( node->right ){
        optimisationConsts( node->right );
    }

    if( optimisationFuncWithTwoArg( node ) ){
        return ;
    }
    if( optimisationFuncWithOneArg( node ) ){
        return ;
    }
}

bool optimisationFuncWithTwoArg( node_t* node ){
    assert( node );

    if( node->left && node->right &&
        node->left->nodeValueType == NUMBER && node->right->nodeValueType == NUMBER ){

        for( size_t indexOfOperators = 0; indexOfOperators < sizeOfMathArray; indexOfOperators++ ){
            if( node->data.mathOperation == arrayWithMathInfo[ indexOfOperators ].mathOperation ){
                double resultOfOptimisation = arrayWithMathInfo[ indexOfOperators ].doMathOperation( node->left->data.number,
                                                                                                     node->right->data.number);
                node->nodeValueType = NUMBER;
                node->data.number = resultOfOptimisation;

                deleteL( node );
                deleteR( node );
                return true;
            }
        }
    }
    return false;
}

bool optimisationFuncWithOneArg( node_t* node ){
    assert( node );

    if( node->right && node->left == NULL && node->right->nodeValueType == NUMBER ){

        for( size_t opWithOneArg = 0; opWithOneArg < sizeOfMathArray; opWithOneArg++ ){
            if( node->data.mathOperation == arrayWithMathInfo[ opWithOneArg ].mathOperation ){
                double resultOfOptimisation = arrayWithMathInfo[ opWithOneArg ].doMathOperation( 0  ,
                                                                                                    node->right->data.number);
                node->nodeValueType = NUMBER;
                node->data.number = resultOfOptimisation;

                deleteR( node );
                return true;
            }
        }
    }
    return false;
}

void removingNeutralElements( tree_t* treeForOptimisation ){
    assert( treeForOptimisation );

    optimisationNeutralElem( treeForOptimisation->rootTree );
}

void optimisationNeutralElem( node_t* node ){
    assert( node );

    if( node->nodeValueType == NUMBER || node->nodeValueType == VARIABLE ){
        return ;
    }

    if( node->left ){
        optimisationNeutralElem( node->left );
    }
    if( node->right ){
        optimisationNeutralElem( node->right );
    }

    if( mulOnZero( node ) )                 return;
    if( mulOnSingleLeftNode( node ) )       return;
    if( mulOnSingleRightNode( node ) )      return;
    if( divideZeroByNum( node ) )           return;
    if( addWithLeftZeroNode( node ) )       return;
    if( addWithRightZeroNode( node ) )      return;

}

bool mulOnZero( node_t* node ){
    assert( node );

    if(node->nodeValueType == OPERATOR && node->data.mathOperation == MUL &&
      (node->left->nodeValueType == NUMBER && fabs( node->left->data.number ) < epsilon ||
       node->right->nodeValueType == NUMBER && fabs( node->right->data.number ) < epsilon )){


        node->nodeValueType = NUMBER;
        node->data.number = 0;

        deleteL( node );
        deleteR( node );
        return true;
    }
    return false;
}

bool mulOnSingleLeftNode( node_t* node ){
    assert( node );

    if( node->nodeValueType == OPERATOR && node->data.mathOperation == MUL && node->left->nodeValueType == NUMBER &&
             fabs( node->left->data.number  - 1 ) < epsilon ){

            if( node->parent->right == node ){
                node->parent->right = copyR( node );
                deleteNode( node );
            }
            else if( node->parent->left == node ){
                node->parent->left = copyR( node );
                deleteNode( node );
            }
            return true;
    }
    return false;
}

bool mulOnSingleRightNode( node_t* node ){
    assert( node );

    if( node->nodeValueType == OPERATOR && node->data.mathOperation == MUL && node->right->nodeValueType == NUMBER &&
             fabs( node->right->data.number  - 1 ) < epsilon ){

            if( node->parent->right == node ){
                node->parent->right = copyL( node );
                deleteNode( node );
            }
            else if( node->parent->left == node ){
                node->parent->left = copyL( node );
                deleteNode( node );
            }
            return true;
    }
    return false;
}

bool divideZeroByNum( node_t* node ){
    assert( node );

    if( node->nodeValueType == OPERATOR && node->data.mathOperation == DIV && node->left->nodeValueType == NUMBER &&
             fabs( node->left->data.number ) < epsilon ){

            node->nodeValueType = NUMBER;
            node->data.number = 0;

            deleteL( node );
            deleteR( node );
            return true;
    }
    return false;
}

bool addWithLeftZeroNode( node_t* node ){
    assert( node );

    if( node->nodeValueType == OPERATOR && node->data.mathOperation == ADD && node->left->nodeValueType == NUMBER &&
            fabs( node->left->data.number ) < epsilon ){

            if( node->parent->right == node ){
                node->parent->right = copyR( node );
                deleteNode( node );
            }
            else if( node->parent->left == node ){
                node->parent->left = copyR( node );
                deleteNode( node );
            }
            return true;
    }
    return false;
}

bool addWithRightZeroNode( node_t* node ){
    assert( node );

    if( node->nodeValueType == OPERATOR && node->data.mathOperation == ADD && node->right->nodeValueType == NUMBER &&
             fabs( node->right->data.number ) < epsilon ){

            if( node->parent->right == node ){
                node->parent->right = copyL( node );
                deleteNode( node );
            }
            else if( node->parent->left == node ){
                node->parent->left = copyL( node );
                deleteNode( node );
            }
            return true;
    }
    return false;
}

mathErrors calculateTheFunctionValue( tree_t* tree ){
    if( tree == NULL ){
        return NULL_PTR;
    }

    double resultOfCalculate = calculateValue( tree->rootTree );

    printf( "\n\nResult of calculate = %lg\n", resultOfCalculate );

    return CORRECT_CALCULATE;
}

double calculateValue( node_t* node ){
    if( node == NULL ){
        return 0;
    }

    if( node->nodeValueType == NUMBER ){
        return node->data.number;
    }
    else if( node->nodeValueType == VARIABLE ){
        return arrayWithVariableValue[ node->data.variableIndexInArray ];
    }

    double firstNumber = calculateValue( node->left );
    double secondNumber = calculateValue( node->right );

    return doMathOperations( node, firstNumber, secondNumber );
}

double doMathOperations( node_t* node, double firstNumber, double secondNumber ){
    assert( node );

    for( size_t functionIndex = 0; functionIndex < sizeOfMathArray; functionIndex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ functionIndex ].mathOperation ){
            return arrayWithMathInfo[ functionIndex ].doMathOperation( firstNumber, secondNumber );
        }
    }

    return 0;
}

void destroyArrayWithVariables(){
    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        free( arrayWithVariables[ varIndex ].nameOfVariable );
    }

    free( arrayWithVariables );
    free( arrayWithVariableValue );
}

void printArrayWithVariablesInFile(){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save info about variables: " );

    char* nameOfFileForVar = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForVar, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ;
    }

    FILE* fileForVar = fopen( nameOfFileForVar, "w" );
    fprintf( fileForVar, "array with variables[ %lu ] = {\n", infoForVarArray.freeIndexNow );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        fprintf( fileForVar, "\t{ %s  , %lu  },\n", arrayWithVariables[ varIndex ].nameOfVariable, arrayWithVariables[ varIndex ].variableIndexInArray );
    }
    fprintf( fileForVar, "};\ncapacity = %lu\n\n", infoForVarArray.capacity );

    for( size_t varIndex = 0; varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        fprintf( fileForVar, "var[%lu] = %lg\n", varIndex, arrayWithVariableValue[ varIndex ] );
    }

    free( nameOfFileForVar );
    fclose( fileForVar );
}

double doAdd( double firstNumber, double secondNumber ){
    return firstNumber + secondNumber;
}
double doSub( double firstNumber, double secondNumber ){
    return firstNumber - secondNumber;
}
double doMul( double firstNumber, double secondNumber ){
    return firstNumber * secondNumber;
}
double doDiv( double firstNumber, double secondNumber ){
    return firstNumber / secondNumber;
}
double doLn( double firstNumber, double secondNumber ){
    return log( secondNumber );
}
double doPow( double firstNumber, double secondNumber ){
    return pow( firstNumber, secondNumber );
}
double doSin( double firstNumber, double secondNumber ){
    return sin( secondNumber );
}
double doCos( double firstNumber, double secondNumber ){
    return cos( secondNumber );
}
double doTg( double firstNumber, double secondNumber ){
    return tan( secondNumber );
}
double doCtg( double firstNumber, double secondNumber ){
    return ( 1 / tan( secondNumber ) );
}
double doArcsin( double firstNumber, double secondNumber ){
    return asin( secondNumber );
}
double doArccos( double firstNumber, double secondNumber ){
    return acos( secondNumber );
}
double doArctg( double firstNumber, double secondNumber ){
    return atan( secondNumber );
}
double doArcctg( double firstNumber, double secondNumber ){
    return ( 1 / atan( secondNumber ) );
}
double doSh( double firstNumber, double secondNumber ){
    return sinh( secondNumber );
}
double doCh( double firstNumber, double secondNumber ){
    return cosh( secondNumber );
}
double doTh( double firstNumber, double secondNumber ){
    return tanh( secondNumber );
}
double doCth( double firstNumber, double secondNumber ){
    return ( 1 / tanh( secondNumber ) );
}
double doExp( double firstNumber, double secondNumber ){
    return exp( secondNumber );
}
double doSqrt( double firstNumber, double secondNumber ){
    return sqrt( secondNumber );
}
