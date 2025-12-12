#include "stack.h"
#include "softProcessor.h"

#ifdef NDEBUG
#define SPU_OK( SPU ) do {} while(false)
#else

void processorDump( const char* file, const char* func, int line, Processor *SPU );
processorError processorVerify( Processor* SPU );

#define SPU_OK( SPU, typeOfErr )                                                                    \
    do{                                                                                             \
        if( processorVerify( SPU ) != CORRECT_SPU ){                                                \
            ( processorDump( __FILE__, __func__, __LINE__, SPU ) );                                 \
            return typeOfErr;                                                                       \
        }                                                                                           \
    }while( false )


#endif
