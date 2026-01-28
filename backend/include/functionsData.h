#ifndef H_FUNCTIONS_DATA
#define H_FUNCTIONS_DATA

#include "tree.h"

enum workWithFunctionsData {
    CORRECT_INITIALIZATION                  = 0,
    ERROR_OF_INITIALIZATION_FUCTIONS_ARRAY  = 1,
    ERROR_OF_INITIALIZATION_FUNCTIONS_SIZE  = 2,
    CORRECT_DESTROY                         = 3,
    CORRECT_SPLIT                           = 4,
    CORRECT_REALLOC                         = 5
};

struct functionInfo {
    size_t currentFunctionIndex;
    size_t countOfFunction;
    size_t capacityOfFuncArray;
};

struct informationWithVariables {
    char* nameOfVariable;
    size_t variableIndexInArray;
    node_t* nodeAddress;
};

struct informationWithVariablesArray {
    size_t capacity;
    size_t freeIndexNow;
    size_t maximumIndexOfVariable;
};

struct functionAndLabels {
    char* nameOfFunction;
    int label;
};

extern informationWithVariables** arrayWithInfoForFunctions;
extern informationWithVariablesArray* arrayWithSizeOfEveryFunctions;
extern functionAndLabels* arrayWithFunctionAndLabels;
extern functionInfo functionInformations;
extern const size_t countOfFunctionsForInitialization;

workWithFunctionsData initFunctionsData( const size_t startIndex, const size_t oldIndex );

workWithFunctionsData isEnoughSizeForFunctionArray();

workWithFunctionsData reallocFunctionsData( size_t oldSize, size_t newSize );

void dumpFunctionArrayInFile();

void dumpDataAboutOneFunction( const size_t functionIndex, FILE* fileForDump );

workWithFunctionsData destroyVariablesData( const size_t functionIndex );

workWithFunctionsData destroyFunctionsData();

void makeArrayWithFunctionAndLabels();

void destroyArrayWithFunctionAndLabels();

#endif
