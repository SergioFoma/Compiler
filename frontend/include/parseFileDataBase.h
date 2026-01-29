#ifndef H_PARSEFILEDATABASE
#define H_PARSEFILEDATABASE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "lexer.h"

enum expertSystemErrors {
    CORRECT_WORK            = 0,
    CAN_NOT_DO_ALLOCATION   = 1,
    ERROR_WITH_GETLINE      = 2,
    NOT_CORRECT_WORK        = 3,
    ERROR_OF_CLEAN          = 4,
    TREE_NULL_PTR           = 5,
    TREE_ERRORS             = 6,
    NULL_PTR_IN_FUNC        = 7,
    ERROR_WITH_FILE         = 8,
    ROOT_NULL_PTR           = 9
};

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree );

node_t* getGeneral( infoForCreateTree* infoForTree );

node_t* makeTreeFromOperators( infoForCreateTree* infoForTree );

node_t* getOperator( infoForCreateTree* infoForTree );

node_t* getNodeFromOperatorThatCalledOperator( infoForCreateTree* infoForTree );

node_t* getAssignment( infoForCreateTree* infoForTree );

node_t* getCondition( infoForCreateTree* infoForTree );

node_t* getCycle( infoForCreateTree* infoForTree );

node_t* getPrint( infoForCreateTree* infoForTree );

node_t* getFunctionDefinition( infoForCreateTree* infoForTree );

node_t* getFunctionDeclaration( infoForCreateTree* infoForTree );

node_t* getReturn( infoForCreateTree* infoForTree );

node_t* getArgumentInFunction( infoForCreateTree* infoForTree );

node_t* getInput( infoForCreateTree* infoForTree );

const char* getEndOfAssignment();

node_t* getExpression( infoForCreateTree* infoForTree );

node_t* getElse( infoForCreateTree* infoForTree );

node_t* getLogicalAnd( infoForCreateTree* infoForTree );

node_t* getLogicalOr( infoForCreateTree* infoForTree );

node_t* getTypeOfEqual( infoForCreateTree* infoForTree );

node_t* getCompareOperations( infoForCreateTree* infoForTree );

node_t* getTerm( infoForCreateTree* infoForTree );

node_t* getPow( infoForCreateTree* infoForTree );

node_t* getFunctionWithOneArgument( infoForCreateTree* infoForTree );

node_t* getPrimaryExpression( infoForCreateTree* infoForTree );

node_t* getVariable( infoForCreateTree* infoForTree );

bool checkingOnFunction( infoForCreateTree* infoForTree );

bool checkingOnStatement( infoForCreateTree* infoForTree );

bool checkingOnExpression( infoForCreateTree* infoForTree );

node_t* getNumber( infoForCreateTree* infoForTree );

#endif

