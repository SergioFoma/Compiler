#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

#include "functionsData.h"

const size_t startVariablesIndex = 0;
const size_t countOfFunctionsForInitialization = 0;

functionInfo functionInformations = { 0, 1, 2 };

informationWithVariables** arrayWithInfoForFunctions = (informationWithVariables**)
calloc( functionInformations.capacityOfFuncArray, sizeof(informationWithVariables*) );

informationWithVariablesArray* arrayWithSizeOfEveryFunctions = (informationWithVariablesArray*)
calloc( functionInformations.capacityOfFuncArray, sizeof(informationWithVariablesArray) );

functionAndLabels* arrayWithFunctionAndLabels = NULL;

workWithFunctionsData initFunctionsData( const size_t startIndex, const size_t finishIndex ){

    for( size_t functionsSize = startIndex; functionsSize < finishIndex; functionsSize++ ){
        arrayWithSizeOfEveryFunctions[ functionsSize ] = { startVariablesIndex + 1, startVariablesIndex, startVariablesIndex };
    }

    for( size_t functionsData = startIndex; functionsData < finishIndex; functionsData++ ){
        arrayWithInfoForFunctions[ functionsData ] = (informationWithVariables*)calloc( startVariablesIndex + 1, sizeof( informationWithVariables ) );
    }

    return CORRECT_INITIALIZATION;
}

workWithFunctionsData isEnoughSizeForFunctionArray(){

    if( functionInformations.countOfFunction == functionInformations.capacityOfFuncArray - 1 ){
        size_t oldCapacity = functionInformations.capacityOfFuncArray;
        functionInformations.capacityOfFuncArray *= 2;
        reallocFunctionsData( oldCapacity, functionInformations.capacityOfFuncArray );
    }

    return CORRECT_REALLOC;
}

workWithFunctionsData reallocFunctionsData( size_t oldSize, size_t newSize ){

    arrayWithInfoForFunctions = (informationWithVariables**)realloc( arrayWithInfoForFunctions, newSize * sizeof(informationWithVariables*) );

    arrayWithSizeOfEveryFunctions = (informationWithVariablesArray*)realloc( arrayWithSizeOfEveryFunctions , newSize * sizeof(informationWithVariablesArray) );

    initFunctionsData( oldSize, newSize );

    return CORRECT_REALLOC;
}

void dumpFunctionArrayInFile( const char* fileNameForFuncDump ){

    FILE* fileForFunctionDump = fopen( fileNameForFuncDump, "w" );

    for( size_t functionIndex = 0; functionIndex < functionInformations.countOfFunction; functionIndex++ ){
        dumpDataAboutOneFunction( functionIndex, fileForFunctionDump );
        fprintf( fileForFunctionDump, "\n\n" );
    }

    fprintf( fileForFunctionDump, "count of functions = %lu\ncapacity of array = %lu\n\nAll functions and they labels:\n",
             functionInformations.countOfFunction, functionInformations.capacityOfFuncArray );

    for( size_t funcIndex = 0; funcIndex < functionInformations.countOfFunction - 1; funcIndex++ ){
        fprintf( fileForFunctionDump, "{ %s, %d },\n",
                 arrayWithFunctionAndLabels[ funcIndex ].nameOfFunction, arrayWithFunctionAndLabels[ funcIndex ].label );
    }

    fclose( fileForFunctionDump );
}

void dumpDataAboutOneFunction( const size_t functionIndex, FILE* fileForDump ){
    assert( fileForDump );

    informationWithVariables* varArray = arrayWithInfoForFunctions[ functionIndex ];
    informationWithVariablesArray sizeVarArray = arrayWithSizeOfEveryFunctions[ functionIndex ];

    fprintf( fileForDump, "array №%lu = {\n", functionIndex );

    for( size_t varIndex = 0; varIndex < sizeVarArray.freeIndexNow; varIndex++ ){
        fprintf( fileForDump, "\t{ %s, %lu, %p },\n",
                 varArray[ varIndex ].nameOfVariable, varArray[ varIndex ].variableIndexInArray, varArray[ varIndex ].nodeAddress);
    }

    fprintf( fileForDump, "};\ncount of variables = %lu\ncapacity = %lu\nmaximumIndexOfVariable = %lu\n",
             sizeVarArray.freeIndexNow, sizeVarArray.capacity, sizeVarArray.maximumIndexOfVariable );
}

workWithFunctionsData destroyVariablesData( const size_t functionIndex ){

    for( size_t varIndex = 0; varIndex < arrayWithSizeOfEveryFunctions[ functionIndex ].freeIndexNow; varIndex++ ){
        free( arrayWithInfoForFunctions[functionIndex][ varIndex ].nameOfVariable );
    }

    arrayWithSizeOfEveryFunctions[ functionIndex ] = { startVariablesIndex + 1, startVariablesIndex, startVariablesIndex };

    return CORRECT_DESTROY;
}

workWithFunctionsData destroyFunctionsData(){

    for( size_t functionsData = 0; functionsData < functionInformations.capacityOfFuncArray; functionsData++ ){
        if( functionsData < functionInformations.countOfFunction ){
            destroyVariablesData( functionsData );
        }
        free( arrayWithInfoForFunctions[ functionsData ] );
    }

    free( arrayWithSizeOfEveryFunctions );
    free( arrayWithInfoForFunctions );

    return CORRECT_DESTROY;
}

void makeArrayWithFunctionAndLabels(){

    arrayWithFunctionAndLabels = (functionAndLabels*)calloc( functionInformations.countOfFunction - 1, sizeof(functionAndLabels) );

    for( size_t funcIndex = 0; funcIndex < functionInformations.countOfFunction - 1; funcIndex++ ){
        arrayWithFunctionAndLabels[ funcIndex ] = { arrayWithInfoForFunctions[ funcIndex + 1 ][0].nameOfVariable, -1 };
    }
}

void destroyArrayWithFunctionAndLabels(){

    for( size_t funcIndex = 0; funcIndex < functionInformations.countOfFunction - 1; funcIndex++ ){
        arrayWithFunctionAndLabels[ funcIndex ].nameOfFunction = NULL; // not free, because that was free in destroyVariablesData
    }

    free( arrayWithFunctionAndLabels );
}
