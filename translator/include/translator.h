#ifndef H_TRANSLATOR
#define H_TRANSLATOR

enum translationErrors {
    SUCCESSFUL_TRANSLATION      = 0,
    ORIGIN_LANG_NULL_PTR        = 1,
    TRANSLATE_LANG_NULL_PTR     = 2

};

translationErrors translateLanguageOnRussian( char* fileWithOriginalLanguage, char* fileWithLanguageOnRussian );

translationErrors translateOnRussian( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateComment( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateExpression( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateStatement( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateMathOperator( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateNumber( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateVariable( FILE* translateLanguage, char** ptrOnSymbolInPosition );

bool translateSpecialSymbol( FILE* translateLanguage, char** ptrOnSymbolInPosition );

#endif
