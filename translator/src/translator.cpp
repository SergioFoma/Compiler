#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "translator.h"
#include "paint.h"
#include "parseFileDataBase.h"
#include "mathOperatorsInfo.h"
#include "myStringFunction.h"
#include "fileBufferInfo.h"

const char* commentBegin  = "##!";
const char* commentEnd = "!##";
size_t commentLen = sizeof( commentBegin ) - 1;

char* getNameOfFile( const char* sentenceForPrint ){
    assert( sentenceForPrint );

    colorPrintf( NOMODE, YELLOW, "%s", sentenceForPrint );

    char* nameOfFileFromKeyboard = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileFromKeyboard, &sizeOfAllocationMemory, stdin );

    return nameOfFileFromKeyboard;
}

translationErrors translateLanguageOnRussian( char* fileWithOriginalLanguage, char* fileWithLanguageOnRussian ){
    assert( fileWithOriginalLanguage );
    assert( fileWithLanguageOnRussian );

    FILE* originalLanguage = fopen( fileWithOriginalLanguage, "r" );
    if( originalLanguage == NULL ){
        return ORIGIN_LANG_NULL_PTR;
    }

    FILE* translateLanguage = fopen( fileWithLanguageOnRussian, "w" );
    if( translateLanguage == NULL ){
        return TRANSLATE_LANG_NULL_PTR;
    }

    bufferInformation bufferWithOriginalLanguage = {};
    initBufferInformation( &bufferWithOriginalLanguage, originalLanguage, fileWithOriginalLanguage );

    char* ptrOnSymbolInPosition = bufferWithOriginalLanguage.buffer;
    translationErrors statusOfTranslate = translateOnRussian( translateLanguage, &ptrOnSymbolInPosition );

    destroyBufferInformation( &bufferWithOriginalLanguage );
    return SUCCESSFUL_TRANSLATION;
}

translationErrors translateOnRussian( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    while( **ptrOnSymbolInPosition != '\0' ){

        if( translateComment( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }

        if( translateExpression( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }

        if( translateStatement( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }

        if( translateMathOperator( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }


        if( translateNumber( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }

        if( translateVariable( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;

        }

        if( translateSpecialSymbol( translateLanguage, ptrOnSymbolInPosition ) == true ){
            continue;
        }

        colorPrintf( NOMODE, RED, "\n\nERROR OF TRANSLATE\n\nNOT UNDEFINED SYMBOL  '%c' \n", **ptrOnSymbolInPosition );
        exit( 0 );
    }

    return SUCCESSFUL_TRANSLATION;
}

bool translateComment( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( strncmp( *ptrOnSymbolInPosition, commentBegin, commentLen ) == 0 ){
        fprintf( translateLanguage, "%s", commentBegin );
        *ptrOnSymbolInPosition += commentLen;

        return true;
    }

    if( strncmp( *ptrOnSymbolInPosition, commentEnd, commentLen ) == 0 ){
        fprintf( translateLanguage, "%s", commentEnd );
        *ptrOnSymbolInPosition += commentLen;

        return true;
    }

    return false;
}

bool translateExpression( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        const char* nameOfOperator = arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile;
        size_t lenOfOperator = strlen( arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile );

        if( strncmp( *ptrOnSymbolInPosition, nameOfOperator, lenOfOperator ) == 0 ){

            fprintf( translateLanguage, "%s", arrayWithExpressions[ expressionIndex ].translationIntoRussian );
            (*ptrOnSymbolInPosition) += lenOfOperator;

            return true;
        }
    }

    return false;
}

bool translateStatement( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        const char* nameOfStatement = arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        size_t lenOfStatement = strlen( arrayWithStatements[ statementIndex ].viewOfStatementInFile );

        if( strncmp( *ptrOnSymbolInPosition, nameOfStatement, lenOfStatement ) == 0 ){

            fprintf( translateLanguage, "%s", arrayWithStatements[ statementIndex ].translationIntoRussian );
            (*ptrOnSymbolInPosition) += lenOfStatement;

            return true;
        }
    }

    return false;
}

bool translateMathOperator( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    for( size_t mathIndex = 0; mathIndex < sizeOfStatementArray; mathIndex++ ){
        const char* nameOfMath = arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile;
        size_t lenOfMath = strlen( arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile );

        if( strncmp( *ptrOnSymbolInPosition, nameOfMath, lenOfMath ) == 0 ){

            fprintf( translateLanguage, "%s", arrayWithMathInfo[ mathIndex ].translationIntoRussian );
            (*ptrOnSymbolInPosition) += lenOfMath;

            return true;
        }
    }

    return false;
}

bool translateNumber( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if ( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' ){

        while( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' ){
            fprintf( translateLanguage, "%c", **ptrOnSymbolInPosition );
            ++( *ptrOnSymbolInPosition );
        }

        return true;
    }

    return false;
}

bool translateVariable( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( isalpha( **ptrOnSymbolInPosition ) || **ptrOnSymbolInPosition == '_' ){
        char* lineWithVar = NULL;
        size_t lineLen = readingVariable( &lineWithVar, ptrOnSymbolInPosition );

        fprintf( translateLanguage, "%s", lineWithVar );
        *ptrOnSymbolInPosition += lineLen;

        free( lineWithVar );
        return true;
    }

    return false;
}

bool translateSpecialSymbol( FILE* translateLanguage, char** ptrOnSymbolInPosition ){
    assert( translateLanguage );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( isspace( **ptrOnSymbolInPosition ) || **ptrOnSymbolInPosition == '\n' || **ptrOnSymbolInPosition == '\t' ){
        while( isspace( **ptrOnSymbolInPosition ) || **ptrOnSymbolInPosition == '\n' || **ptrOnSymbolInPosition == '\t' ){
            fprintf( translateLanguage, "%c", **ptrOnSymbolInPosition );
            ++( *ptrOnSymbolInPosition );
        }

        return true;
    }

    return false;
}
