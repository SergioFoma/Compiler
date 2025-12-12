#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "assembleCode.h"
#include "stringFunction.h"
#include "paint.h"

commandForPrint arrayWithOneCommand[] = {
    { "PUSH",  PUSH },
    { "IN",    IN },
    { "MUL",   MUL },
    { "SQRT", SQRT },
    { "SUB",   SUB },
    { "ADD",   ADD },
    { "OUT",   OUT },
    { "HLT",   HLT },
    { "DRAW",  DRAW },
    { "DIV",   DIV  },
    { "MOD",   MOD },
    { "JMP",   JMP },
    { "RET",   RET },
};
size_t sizeArrayWithOneCommand = sizeof( arrayWithOneCommand ) / sizeof( arrayWithOneCommand[ 0 ] );

commandForLabel arrayWithLabelCommand[] = {
    { "JB", JB,  checkLabel },
    { "JBE", JBE, checkLabel },
    { "JA", JA, checkLabel },
    { "JAE", JAE, checkLabel },
    { "JE", JE, checkLabel },
    { "JNE", JNE, checkLabel },
    { "CALL", CALL, checkLabel },
};
size_t sizeArrayWithLabelCommand = sizeof( arrayWithLabelCommand ) / sizeof( arrayWithLabelCommand[ 0 ] );

commandForRegs arrayWithRegsCommand[] = {
    { "PUSHR", PUSHR, parsingRegsCommand },
    { "POPR",  POPR, parsingRegsCommand }
};
size_t sizeArrayWithRegsCommand = sizeof( arrayWithRegsCommand ) / sizeof( arrayWithRegsCommand[ 0 ] );

typeOfRegs arrayWithRegs[] = {
    { "RAX",   RAX },
    { "RBX",   RBX },
    { "RCX",   RCX },
    { "RDX",   RDX }
};
size_t sizeArrayWithRegs = sizeof( arrayWithRegs ) / sizeof( arrayWithRegs[ 0 ] );

commandForRam arrayWithRamCommand[] = {
    { "PUSHM", PUSHM, parsingRamCommand },
    { "POPM",  POPM, parsingRamCommand }
};
size_t sizeArrayWithRamCommand = sizeof( arrayWithRamCommand ) / sizeof( arrayWithRamCommand[ 0 ] );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels){
    assert( fileForAsm != NULL );
    assert( fileForByteCode != NULL );
    assert( labels != NULL );

    FILE* asmFile = fopen( fileForAsm, "r");

    bufferInformation bufferFromFile = {};
    strInformation stringFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, asmFile, fileForAsm );
    if( bufferError != correct ){
        return INIT_BUFFER_ERROR;
    }

    errorCode stringError = initStringInformation( &stringFromFile );
    if( stringError != correct ){
        return INIT_STRING_ERROR;
    }

    if ( !splitToLines( &bufferFromFile, &stringFromFile, asmFile ) ){
        return SPLIT_LINES_ERROR;
    }
    fclose( asmFile );

    informationOfStringCommand stringCommand = {};
    typeOfErr err = initStringCommand( &stringCommand );
    if( err != OK ){
        return err;
    }

    err = splitToOperands( stringFromFile, &stringCommand );
    if( err != OK ){
        return err;
    }

    int* commandArray = (int*)calloc( stringCommand.arraySize, sizeof( int ) );
    if( commandArray == NULL ){
        return NULL_PTR;
    }

    FILE* byteFile = fopen( fileForByteCode, "a" );
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }

    bool doSecondPass = false;
    size_t sizeOfEncodeCommands = encodeCommands( stringCommand, byteFile, labels, commandArray, &doSecondPass );
    if( sizeOfEncodeCommands == 0 ){
        return UNIDENTIFIED_COMMAND;
    }

    if( doSecondPass ){
        sizeOfEncodeCommands = encodeCommands( stringCommand, byteFile, labels, commandArray, NULL );
        if( sizeOfEncodeCommands == 0 ){
            return UNIDENTIFIED_COMMAND;
        }
    }

    err = printInFile( byteFile, commandArray, sizeOfEncodeCommands );
    if( err != OK ){
        return err;
    }

    fclose( byteFile );
    free( commandArray );
    destroyStringCommand( &stringCommand );

    return OK;
}

void parseCommandName( char* lineFromFile ){
    assert( lineFromFile != NULL );

    char* getIndexOfComment = strchr( lineFromFile, ';' );
    if( getIndexOfComment != NULL ){
        *getIndexOfComment = '\0';
    }
}

void asmPrintf( FILE* byteFile,  int intFirstArg){
    assert( byteFile != NULL );

    fprintf( byteFile, "%d ", intFirstArg);
}

typeOfErr splitToOperands( strInformation stringFromFile, informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        colorPrintf( NOMODE, RED, "\nstruct stringCommand = NULL PTR in :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return COMMAND_NULL_PTR;
    }

    int statusOfReading = -1;
    char commandName[ lenOfCommand ] = "", commandCode[ lenOfCommand ] = "";

    for( size_t stringArrayIndex = 0; stringArrayIndex < stringFromFile.arraySize; stringArrayIndex++ ){
        parseCommandName( stringFromFile.arrayOfStr[ stringArrayIndex ] );

        statusOfReading = sscanf( stringFromFile.arrayOfStr[ stringArrayIndex ], "%s %s", commandName, commandCode );
        if( (stringCommand->arraySize + statusOfReading) >= stringCommand->startSize ){
            stringCommand->startSize *= 2;
            stringCommand->arrayWithStringCommand= (char**)realloc( stringCommand->arrayWithStringCommand,
                                                      (stringCommand->startSize) * sizeof( char* ) );
            if( stringCommand->arrayWithStringCommand == NULL ){
                return COMMAND_NULL_PTR;
            }
        }
        if( statusOfReading == 1 ){
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandName);
        }
        else if( statusOfReading == 2 ){
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandName);
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandCode);
        }
    }

    return OK;
}

typeOfErr initStringCommand( informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        return INIT_COMMAND_ERROR;
    }

    stringCommand->startSize = 8;
    stringCommand->arrayWithStringCommand = (char**)calloc( stringCommand->startSize, sizeof( char* ) );
    stringCommand->arraySize = 0;
    return OK;
}

typeOfErr destroyStringCommand( informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        return COMMAND_NULL_PTR;
    }

    for( size_t stringCommandIndex = 0; stringCommandIndex < (stringCommand->arraySize); stringCommandIndex++){
        free( (stringCommand->arrayWithStringCommand)[ stringCommandIndex ] );
    }

    free( stringCommand->arrayWithStringCommand );
    stringCommand->arraySize = 0;
    stringCommand->startSize = 0;
    return OK;
}

size_t encodeCommands( informationOfStringCommand stringFromFile, FILE* byteFile, int* labels, int* commandArray, bool* doSecondPass ){
    assert( byteFile != NULL );
    assert( labels != NULL );
    assert( commandArray != NULL );

    bool flag = false, resultOfCheckingSecondPass = false;
    size_t stringIndex = 0, intIndex = 0;
    typeOfErr errorFromParsingCommand = UNIDENTIFIED_COMMAND;

    while( stringIndex < stringFromFile.arraySize){
        flag = false;

        flag = checkNumber( stringFromFile, &stringIndex, commandArray, &intIndex);
        GO_NEXT_OR_NOT();

        flag = findLabel( stringFromFile, &stringIndex, labels, commandArray, &intIndex );
        GO_NEXT_OR_NOT();

        for( size_t labelIndex = 0; labelIndex < sizeArrayWithLabelCommand; labelIndex++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithLabelCommand[ labelIndex ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithLabelCommand[ labelIndex ].intFirstArg;
                ++stringIndex;
                resultOfCheckingSecondPass = arrayWithLabelCommand[ labelIndex ].func( stringFromFile, &stringIndex, labels, commandArray, &intIndex );
                if( resultOfCheckingSecondPass == true ) {
                    *doSecondPass = resultOfCheckingSecondPass;
                }
                flag = true;
                break;
            }
        }
        GO_NEXT_OR_NOT();

        for( size_t regsIndex = 0; regsIndex < sizeArrayWithRegsCommand; regsIndex++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithRegsCommand[ regsIndex ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithRegsCommand[ regsIndex ].intFirstArg;
                ++stringIndex;
                errorFromParsingCommand = arrayWithRegsCommand[ regsIndex ].func( stringFromFile, &stringIndex, commandArray, &intIndex );
                if( errorFromParsingCommand == OK ){
                    flag = true;
                    break;
                }

            }
        }
        GO_NEXT_OR_NOT();

        for( size_t ramIndex = 0; ramIndex < sizeArrayWithRamCommand; ramIndex++ ){
            if( strcmp( stringFromFile.arrayWithStringCommand[ stringIndex ], arrayWithRamCommand[ ramIndex ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithRamCommand[ ramIndex ].intFirstArg;
                ++stringIndex;
                errorFromParsingCommand = arrayWithRamCommand[ ramIndex ].func( stringFromFile, &stringIndex, commandArray, &intIndex );
                if( errorFromParsingCommand == OK ){
                    flag = true;
                    break;
                }
            }
        }
        GO_NEXT_OR_NOT();

        for( size_t oneCommandIndex = 0; oneCommandIndex < sizeArrayWithOneCommand; oneCommandIndex++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithOneCommand[ oneCommandIndex ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithOneCommand[ oneCommandIndex ].intFirstArg;
                ++stringIndex;
                flag = true;
                break;
            }

        }
        GO_NEXT_OR_NOT();

        if( errorFromParsingCommand == UNIDENTIFIED_COMMAND ){
            colorPrintf( NOMODE, RED, "\n\nUnidentified command:%s %s %d\n\n", __FILE__, __func__, __LINE__ );
            return 0;
        }
    }

    return intIndex;
}

bool checkNumber( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex ){
    if( stringIndex == NULL || commandArray == NULL || intIndex == NULL ){
        return false;
    }

    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    if( isdigit( command[0]) || command[0] == '-' ){
        commandArray[ (*intIndex)++ ] = atoi( command );
        ++(*stringIndex);
        return true;
    }

    return false;
}

bool checkLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex ){
    assert( stringIndex != NULL );
    assert( labels != NULL );
    assert( commandArray != NULL );
    assert( intIndex != NULL );

    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    commandArray[ (*intIndex)++ ] = labels[ atoi( command + 1 ) ];

    if( *stringIndex < stringFromFile.arraySize - 1 ){
        ++(*stringIndex);
    }
    if( labels[ atoi( command + 1 ) ] == -1 ){
        return true;
    }

    return false;
}

bool findLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex ){
    assert( stringIndex != NULL );
    assert( labels != NULL );
    assert( commandArray != NULL );
    assert( intIndex != NULL );

    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];
    if( command[0] == ':' ){
        labels[ atoi( command + 1 ) ] = *intIndex;
        ++(*stringIndex);
        return true;
    }

    return false;
}
typeOfErr printInFile( FILE* byteFile, int* commandArray, size_t sizeOfIntCommand ){
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }
    else if( commandArray == NULL ){
        return NULL_PTR;
    }

    for( size_t commandIndex = 0; commandIndex < sizeOfIntCommand; commandIndex++ ){
        fprintf( byteFile, "%d ", commandArray[ commandIndex ] );
    }
    return OK;
}

typeOfErr parsingRegsCommand( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex ){
    assert( stringIndex != NULL );
    assert( commandArray != NULL );
    assert( intIndex != NULL );

    char* command = stringFromFile.arrayWithStringCommand[ *stringIndex];
    for( size_t regsIndex = 0; regsIndex < sizeArrayWithRegs; regsIndex++ ){
        if( strcmp( command, arrayWithRegs[ regsIndex ].firstArg ) == 0 ){
            commandArray[ (*intIndex)++ ] = arrayWithRegs[ regsIndex ].intFirstArg;
            ++(*stringIndex);
            return OK;
        }
    }
    colorPrintf( NOMODE, RED, "\n\nUnidentified command:%s %s %d\n\n", __FILE__, __func__, __LINE__ );
    return UNIDENTIFIED_COMMAND;
}

typeOfErr parsingRamCommand( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex ){
    assert( stringIndex != NULL );
    assert( commandArray != NULL );
    assert( intIndex != NULL );

    char* ramCommand = stringFromFile.arrayWithStringCommand[ *stringIndex ];

    char command[ lenOfCommand ] = "";
    parseRegsInRamCommand( ramCommand, command );

    for( size_t ramIndex = 0; ramIndex < sizeArrayWithRegs; ramIndex++ ){
        if( strcmp( command, arrayWithRegs[ ramIndex ].firstArg ) == 0 ){
            commandArray[ (*intIndex)++ ] = arrayWithRegs[ ramIndex ].intFirstArg;
            ++(*stringIndex);
            return OK;
        }
    }
    colorPrintf( NOMODE, RED, "\n\nUnidentified command:%s %s %d\n\n", __FILE__, __func__, __LINE__ );
    return UNIDENTIFIED_COMMAND;
}

void parseRegsInRamCommand( char* ramCommand, char* correctRamCommand ){
    assert( ramCommand != NULL );

    char* bracketIndex = strchr( ramCommand, '[' );
    size_t correctRamIndex = 0;

    while( *( bracketIndex + correctRamIndex + 1) != ']' && correctRamIndex < lenOfCommand ){
        correctRamCommand[ correctRamIndex ] = *( bracketIndex + correctRamIndex + 1);
        ++correctRamIndex;
    }
    correctRamCommand[ correctRamIndex ] = '\0';
}

void analysisOfAssembleError( typeOfErr assembleError ){
    switch( assembleError ){
        case OK:
            break;
        case COMMAND_ERROR:
            colorPrintf( NOMODE, RED, "\nMAIN get error about command array\n" );
            break;
        case BYTE_FILE_ERROR:
            colorPrintf( NOMODE, RED, "\nMain get error about opening byte file\n" );
            break;
        case SPLIT_LINES_ERROR:
            colorPrintf( NOMODE, RED, "\nMain get error about split command lines\n");
            break;
        case INIT_STRING_ERROR:
            colorPrintf( NOMODE, RED, "\nMain get error about init string struct\n");
            break;
        case INIT_BUFFER_ERROR:
            colorPrintf( NOMODE, RED, "\nMain get error about init buffer struct\n");
            break;
        case INIT_COMMAND_ERROR:
            colorPrintf( NOMODE, RED, "\nMain get error about init of array with string command\n");
            break;
        case COMMAND_NULL_PTR:
            colorPrintf( NOMODE, RED, "\nMain get error about null ptr of command\n");
            break;
        case NULL_PTR:
            colorPrintf( NOMODE, RED, "\nMain get error about null ptr\n");
            break;
        case UNIDENTIFIED_COMMAND:
            colorPrintf( NOMODE, RED, "\nMain get erro about founded unidentified command\n");
            break;
        default:
            break;
    }
}
