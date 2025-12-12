#include "stack.h"

#ifdef UNDEBUG
#define STACK_OK( err ) do {} while(false)
#else

void stackDump( const char* file, const char* func, int line, stack_t *stk );
stackErr_t stackVerify( stack_t *stk );

#define STACK_OK( stk )                                              \
    do{                                                              \
        if( stackVerify( stk ) != CORRECT ) {                        \
            ( stackDump( __FILE__, __func__, __LINE__,  stk ) );     \
            return ;                                                 \
        }                                                            \
    }while( false )

#endif
