#ifndef H_TREE
#define H_TREE

#include <limits.h>
#include <float.h>

#define nodeValueFormatForPrint( valueFormat ) valueFormat;
#define maxTreeValue DBL_MAX

const size_t oneStruct = 1;

enum treeErrors {
    CORRECT_TREE                = 0,
    NODE_NULL_PTR               = 1,
    NOT_ENOUGH_MEMORY           = 2,
    ERROR_OF_OPEN_FILE          = 3,
    NOT_CORRECT_TYPE_OF_DATA    = 4,
    ERROR_OF_GET_NAME_OF_FILE   = 5
};

enum typeOfDataInNode {
    NUMBER      = 0,
    OPERATOR    = 1,
    VARIABLE    = 2,
    STATEMENT   = 3,
    EXPRESSION  = 4
};

#define typeOfRoot NUMBER

enum typeOfMathOperation {
    ADD     = 0,
    SUB     = 1,
    MUL     = 2,
    DIV     = 3,
    LOG     = 4,
    LN      = 5,
    POW     = 6,
    SIN     = 7,
    COS     = 8,
    SQRT    = 9,
    TG      = 10,
    CTG     = 11,
    ARCSIN  = 12,
    ARCCOS  = 13,
    ARCTG   = 14,
    ARCCTG  = 15,
    SH      = 16,
    CH      = 17,
    TH      = 18,
    CTH     = 19,
    EXP     = 20
};

enum typeOfStatement {
    NO_TYPE            = 0,
    ASSIGNMENT         = 1,
    OPERATOR_END       = 2,
    IF                 = 3,
    PAR_OPEN           = 4,
    PAR_CLOSE          = 5,
    CURLY_PAR_OPEN     = 6,
    CURLY_PAR_CLOSE    = 7,
    WHILE              = 8,
    ELSE               = 9
};

enum typeOfExpressions {
    AND                     = 0,
    OR                      = 1,
    BELOW                   = 2,
    BELOW_OR_EQUAL          = 3,
    ABOVE                   = 4,
    ABOVE_OR_EQUAL          = 5,
    EQUAL                   = 6,
    NOT_EQUAL               = 7
};

union treeElem_t {
    double number;
    size_t variableIndexInArray;
    typeOfMathOperation mathOperation;
    typeOfStatement statement;
    typeOfExpressions expressionOperator;
};

struct node_t {
    typeOfDataInNode nodeValueType;
    treeElem_t data;
    node_t* left;
    node_t* right;
    node_t* parent;
};

struct tree_t{
    node_t* rootTree;
};

void printNode( const node_t* tree );

void printTree( const tree_t* tree );

void printTheSortedNode( const node_t* tree );

void printTheSortedTree( const tree_t* tree );

treeErrors initTree( tree_t* tree );

treeErrors initNode( node_t** node, typeOfDataInNode typeOfData, treeElem_t element );

treeErrors initializationDataInNode( node_t** node, typeOfDataInNode typeOfData, treeElem_t element  );

treeErrors insertNode( tree_t* root, treeElem_t element );

void destroyNode( node_t* node );

void destroyTree( tree_t* node );

node_t* newStatementNode( typeOfDataInNode nodeType, typeOfStatement typeOfStatement, node_t* leftNode, node_t* rightNode );

node_t* newExpressionNode( typeOfDataInNode nodeType, typeOfExpressions typeOfStatement, node_t* leftNode, node_t* rightNode );

#endif
