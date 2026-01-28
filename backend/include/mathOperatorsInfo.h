#ifndef H_MATH_OPERATORS_INFO
#define H_MATH_OPERATORS_INFO

#include <stdio.h>

#include "tree.h"

enum functionClasses {
    ONE_ARG   = 0,
    TWO_ARG   = 1,
    TRIG      = 2
};

struct informationWithValueType {
    typeOfDataInNode typeOfData;
    const char* nameOfType;
};

extern informationWithValueType arrayWithValueType[];
extern const size_t sizeOfArrayWithValueType;

struct informationWithMathOperators {
    typeOfMathOperation mathOperation;
    const char* nameOfMathOperation;
    const char* viewOfMathOperationInFile;
    functionClasses functionClass;
};

extern informationWithMathOperators arrayWithMathInfo[];
extern const size_t sizeOfMathArray;

struct informationWithStatements {
    typeOfStatement statement;
    const char* nameOfStatement;
    const char* viewOfStatementInFile;
    const char* viewOfStatementInAST;
    size_t(*translateStatementInASMcommand)( const node_t* node, FILE* fileForASM );
};

extern informationWithStatements arrayWithStatements[];
extern const size_t sizeOfStatementArray;

struct informationWithExpressions {
    typeOfExpressions expressionOperator;
    const char* nameOfExpressions;
    const char* viewOfExpressionOperatorInFile;
    const char* viewOfExpressionInAST;
    size_t(*translateExpressionInASMcommand)( const node_t* node, FILE* fileForASM );
};

extern informationWithExpressions arrayWithExpressions[];
extern const size_t sizeOfExpressionArray;

#endif
