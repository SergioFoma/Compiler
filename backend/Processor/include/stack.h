#ifndef H_STACK
#define H_STACK

#include <stdlib.h>
#include <stdbool.h>

typedef int type;

const size_t poison_ = 765911;
const type canary = 179171341; // 0xAADF00D

enum stackErr_t  {
    CORRECT                = 0,
    FILE_NULL_PTR          = 1,
    FUNC_NUL_PTR           = 2,
    DATA_NULL_PTR          = 3,
    STK_NULL_PTR           = 4,
    NEGATIVE_SIZE          = 5,
    NEGATIVE_CAPACITY      = 6,
    NEGATIVE_LINE          = 7,
    STACK_OVERFLOW         = 8,
};

struct stack_t{
    type* data;
    size_t size;
    size_t capacity;
    stackErr_t error;
};

void stackInit( stack_t *stk, size_t startSize);

void stackPush( stack_t *stk, type value );

type stackPop( stack_t *stk );

void stackPrint( stack_t *stk );

void stackDestroy( stack_t *size );

#endif
