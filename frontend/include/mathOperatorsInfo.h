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
};

extern informationWithStatements arrayWithStatements[];
extern const size_t sizeOfStatementArray;

struct informationWithExpressions {
    typeOfExpressions expressionOperator;
    const char* nameOfExpressions;
    const char* viewOfExpressionOperatorInFile;
    const char* viewOfExpressionInAST;
};

struct informationWithVariables {
    char* nameOfVariable;
    size_t variableIndexInArray;
};

struct informationWithVariablesArray {
    size_t capacity;
    size_t freeIndexNow;
};

extern informationWithVariables* arrayWithVariables;
extern informationWithVariablesArray infoForVarArray;
extern double* arrayWithVariableValue;

extern informationWithExpressions arrayWithExpressions[];
extern const size_t sizeOfExpressionArray;

#endif
