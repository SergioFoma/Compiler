#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "softProcessor.h"
#include "stackCheckError.h"
#include "spuCheckError.h"
#include "paint.h"

void processorInit( Processor* SPU ){
    assert( SPU != NULL );

    stackInit( &(SPU->stk), startSizeForStk );
    stackInit( &(SPU->regAddr), startSizeForStk );

    SPU->RAM = (int*)calloc( sizeRam, sizeof( int ) );
    if( SPU->RAM  == NULL ){
        colorPrintf( NOMODE, RED, "\n\nCan not initialization:%s %s %d", __FILE__, __func__, __LINE__ );
        return ;
    }
    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = CORRECT_SPU;
    (SPU->code).sizeOfCommands = 0;
}

void softProcessor( const char* nameOfByteFile, Processor* SPU ){
    assert( SPU != NULL );
    SPU_OK( SPU, (void)0 );

    FILE* byteFile = fopen( nameOfByteFile, "r" );

    bufferInformation bufferFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, byteFile, nameOfByteFile );
    if( bufferError != correct ){
        colorPrintf(NOMODE, RED, "Error of init buffer struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        fclose( byteFile );
        return ;
    }

    getArrayWithCommand( bufferFromFile, SPU );
    fclose( byteFile );
}


void getArrayWithCommand( bufferInformation bufferFromFile, Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    size_t  indexOfBuffer = 0, startSizeForArray = 8;
    int countOfReading = 0;
    (SPU->code).command= (int*)calloc( startSizeForArray, sizeof( int ) );
    if( (SPU->code).command == NULL ){
        colorPrintf( NOMODE, RED, "\n\nNot enough memory:%s %s %d", __FILE__, __func__, __LINE__ );
        return ;
    }

    while ( indexOfBuffer < bufferFromFile.bufferSize ) {
        if( ( (SPU->code).sizeOfCommands ) == startSizeForArray - 1 ){
            startSizeForArray *= 2;
            (SPU->code).command = (int*)realloc( (SPU->code).command , startSizeForArray * sizeof( int ) );
        }

        *( (SPU->code).command + ( (SPU->code).sizeOfCommands)++ ) = atoi( bufferFromFile.buffer + indexOfBuffer );
        countOfReading = strchr( bufferFromFile.buffer + indexOfBuffer, ' ' ) - (bufferFromFile.buffer + indexOfBuffer) + 1;
        if( countOfReading == 0){
            break;
        }
        indexOfBuffer += countOfReading;
    }
}

void processorDestroy( Processor* SPU ){
    assert( SPU != NULL );

    stackDestroy( &(SPU->stk) );
    stackDestroy( &(SPU->regAddr) );
    free( (SPU->code).command );
    free( SPU->RAM );

    (SPU->code).command = NULL;
    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = SPU_DESTROY;
}

void regsPrint( Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    colorPrintf(NOMODE, YELLOW, "regs:\n");
    for( size_t regsIndex = 0; regsIndex < sizeOfRegs; regsIndex++ ){
        colorPrintf(NOMODE, YELLOW, "[ %lu ] = %d ", regsIndex, (SPU->regs)[regsIndex] );
    }
    printf("\n");
}

void ramPrint( Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    colorPrintf( NOMODE, BLUE, "RAM:\n" );
    for( size_t videoIndex = 0; videoIndex < sizeOfVideoMemory; videoIndex++){
        printf("%c ", (SPU->RAM)[ videoIndex ] );
        if( (videoIndex+1) % countOfSymbolInLine == 0 ){
            printf("\n");
        }
    }
    printf("\n");
    sleep(0.9);
}

void doColor( Processor* SPU, size_t index ){
    assert( SPU != NULL );

    switch( (SPU->RAM)[ index ] ){
        case PURPLE_COLOR:
            colorPrintf( NOMODE, PURPLE, "%c ", (SPU->RAM)[ index + 1 ] );
            break;
        case RED_COLOR:
            colorPrintf( NOMODE, RED, "%c ", (SPU->RAM)[ index + 1 ] );
            break;
        default:
            printf( "%c ", (SPU->RAM)[ index + 1 ] );
            break;
    }
}
