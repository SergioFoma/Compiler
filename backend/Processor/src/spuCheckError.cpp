#include <stdio.h>
#include <assert.h>

#include "stack.h"
#include "softProcessor.h"
#include "spuCheckError.h"
#include "stackCheckError.h"

const int poison = 765911;

processorError processorVerify( Processor* SPU ){
    if( SPU == NULL ){
        return SPU_NULL_PTR;
    }
    if( stackVerify( &(SPU->stk) )  != CORRECT ){
        return STACK_ERROR;
    }

    if( SPU->spuErr != CORRECT_SPU ){
        return (SPU->spuErr );
    }

    return CORRECT_SPU;
}

void processorDump( const char* file, const char* func, int line, Processor *SPU ){
    assert( file != NULL );
    assert( func != NULL );

    FILE* fileForSPU = fopen( "ProcessorStatus.txt", "a" );

    if( stackVerify( &(SPU->stk) ) != CORRECT ){
        stackDump( __FILE__, __func__, __LINE__, &(SPU->stk) );
        SPU->spuErr = STACK_ERROR;
        fclose( fileForSPU );
        return ;
    }
    else if( stackVerify( &(SPU->regAddr) ) != CORRECT ){
        stackDump( __FILE__, __func__, __LINE__, &(SPU->regAddr) );
        SPU->spuErr = STACK_ERROR;
        fclose( fileForSPU );
        return ;
    }
    else if( (SPU->code).command == NULL ){
        fprintf( fileForSPU, "\nIn struct code: command - NULL PTR\n");
        SPU->spuErr = CODE_NULL_PTR;
        fclose( fileForSPU );
        return ;
    }
    else if( SPU->spuErr != CORRECT_SPU ){
        fprintf( fileForSPU, "\nIn SPU spuErr = %d\n", SPU->spuErr );
        fclose( fileForSPU );
        return ;
    }

    fprintf( fileForSPU, "\nprocessorDump called from file %s, in function %s, in line %d\n",
              file, func, line);
    fprintf( fileForSPU, "\nSPU[%p]\nCode.code[%p]\nregs[%p]\nspuErr = %d\n",
                          SPU, &((SPU->code).command), &(SPU->regs), SPU->spuErr );
    fprintf( fileForSPU, "\nstack[%p]\nSize = %lu\nCapacity = %lu\n.data[%p]\nCode of error = %d\n",
            &(SPU->stk), (SPU->stk).size, (SPU->stk).capacity, &((SPU->stk).data), (SPU->stk).error );

    size_t index = 0;

    fprintf( fileForSPU, "\nPrint element in stack: \n" );
    for( index = 0; index < ( (SPU->stk).capacity ); index++ ){
        if( index < ((SPU->stk).size) || index == (SPU->stk).capacity - 1 ){
            fprintf( fileForSPU, "*[%lu] = %d\n", index, ((SPU->stk).data)[index] );
        }
        else{
            fprintf( fileForSPU, " [%lu] = %d( POISON )\n", index, poison);
        }
    }

    fprintf( fileForSPU, "\nPrint element in code: \n" );
    for( index = 0; index < ( (SPU->code).sizeOfCommands ); index++ ){
        fprintf( fileForSPU, "[%lu] = %d ", index, ( (SPU->code).command )[index] );
    }

    fprintf( fileForSPU, "\nInstruction Pointer = %ld\n", SPU->instructionPointer );

    fprintf( fileForSPU, "\nPrint element in regs: \n" );
    for( index = 0; index < sizeOfRegs; index++ ){
        fprintf( fileForSPU, "[%lu] = %d ", index, (SPU->regs)[index] );
    }

    fprintf( fileForSPU, "\nIndex of last element in regs = %d\n", SPU->indexForRegister );

    fclose( fileForSPU );
}
