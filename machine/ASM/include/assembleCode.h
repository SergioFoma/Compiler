#ifndef H_ASSEMBLECODE
#define H_ASSEMBLECODE

#include <stdio.h>

#include "stringFunction.h"

const size_t lenOfCommand = 20;

enum typeOfErr {
    OK                   = 0,
    COMMAND_ERROR        = 1,
    BYTE_FILE_ERROR      = 2,
    SPLIT_LINES_ERROR    = 3,
    INIT_STRING_ERROR    = 4,
    INIT_BUFFER_ERROR    = 5,
    INIT_COMMAND_ERROR   = 6,
    COMMAND_NULL_PTR     = 7,
    NULL_PTR             = 8,
    UNIDENTIFIED_COMMAND = 9,
};
enum regsIndex {
    RAX             = 0,
    RBX             = 1,
    RCX             = 2,
    RDX             = 3,
    REX             = 4,
    NOT_IDENTIFIED  = 9
};

enum doCommand {
    HLT         = 0,
    PUSH        = 1,
    MUL         = 2,
    SUB         = 3,
    OUT         = 4,
    ADD         = 5,
    DIV         = 6,
    SQRT        = 7,
    IN          = 9,
    DRAW        = 10,
    MOD         = 11,
    POPR        = 42,
    PUSHR       = 33,
    JB          = 50,
    JBE         = 51,
    JA          = 52,
    JAE         = 53,
    JE          = 54,
    JNE         = 55,
    JMP         = 56,
    CALL        = 60,
    RET         = 61,
    PUSHM       = 70,
    POPM        = 71
};

void asmPrintf( FILE* byteFile,  int intFirstArg );

struct commandForPrint {
    const char* firstArg;
    int intFirstArg;
};
struct informationOfStringCommand {
    char** arrayWithStringCommand;
    size_t arraySize;
    size_t startSize;
};
struct commandForLabel {
    const char* firstArg;
    int intFirstArg;
    bool ( *func )( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex );
};
struct commandForRegs {
    const char* firstArg;
    int intFirstArg;
    typeOfErr ( *func )( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex );
};
struct typeOfRegs {
    const char* firstArg;
    int intFirstArg;
};
struct commandForRam{
    const char* firstArg;
    int intFirstArg;
    typeOfErr ( *func )( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex );
};
struct typeOfRamRegs {
    const char* firstArg;
    int intFirstArg;
};

typeOfErr parsingRegsCommand( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex );

typeOfErr parsingRamCommand( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex );

typeOfErr initStringCommand( informationOfStringCommand* stringCommand );

typeOfErr destroyStringCommand( informationOfStringCommand* stringCommand );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels );

typeOfErr splitToOperands( strInformation stringFromFile, informationOfStringCommand* stringCommand );

size_t encodeCommands( informationOfStringCommand stringFromFile, FILE* byteFile, int* labels, int* commandArray, bool* doSecondPass );

void parseCommandName( char* lineFromFile );

void parseRegsInRamCommand( char* ramCommand, char* correctRamCommand );

typeOfErr writeToFile( strInformation stringFromFile, FILE* byteFile, int* labels, size_t* countOfCommand );

bool findLabels( FILE* byteFile, char* lineFromFile, int* labels, size_t* countOfCommand );

bool checkNumber( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex );

bool checkLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex  );

bool findLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex );

typeOfErr printInFile( FILE* byteFile, int* commandArray, size_t sizeOfIntCommands );

void analysisOfAssembleError( typeOfErr assembleError );

char* getNameOfFileForSPU();

#define GO_NEXT_OR_NOT()            \
    if( flag == true ){             \
        continue;                   \
    }
#endif
