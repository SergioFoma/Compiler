#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "stackCheckError.h"
#include "paint.h"

void stackInit( stack_t *stk, size_t startSize ){
    (stk->data) = (type*)calloc( startSize + 2, sizeof( type ) );

    if( stk->data == NULL ){
        stk->error = DATA_NULL_PTR;
        return ;
    }

    stk->size = 1;
    stk->capacity = startSize + 2;
    (stk->data)[0] = canary;
    (stk->data)[stk->capacity - 1] = canary;
    stk->error = CORRECT;

    return ;
}

void stackPush( stack_t *stk, type value ){
    STACK_OK( stk );

    if( stk->size == stk->capacity - 1 ){
        (stk->capacity) *= 2;
        stk->data = (type*)realloc( stk->data, (stk->capacity) * sizeof( type ) );
        if( stk->data == NULL ){
            stk->error = DATA_NULL_PTR;
            return ;
        }
        stk->data[ stk->capacity - 1] = canary;
    }
    (stk->data)[stk->size++] = value;

    return ;
}

type stackPop( stack_t *stk ){
    if ( stackVerify( stk ) != CORRECT ){
        return poison_;
    }

    if( stk->size <= 1 || stk->capacity <= 1){
        return poison_;
    }
    return (stk->data)[--(stk->size)];
}

void stackPrint( stack_t *stk ){
    STACK_OK( stk );

    colorPrintf(NOMODE, GREEN, "\nSize stack now: %lu \nstack: \n", stk->size - 1);
    for( size_t index = 0; index < (stk->size) - 1; index++ ){
        colorPrintf(NOMODE, GREEN, "[%u] = %d ", index, (stk->data)[index+1] );
    }
    printf("\n");

    return ;
}

void stackDestroy( stack_t *stk ){
    free( stk->data );
    stk->data = NULL;
    stk->capacity = -1;
    stk->size = -1;
    stk->error = CORRECT;

    return ;
}
