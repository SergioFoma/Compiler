#include <stdio.h>
#include <stdlib.h>

#include "translator.h"

int main(){

    const char* sentenceForOriginalLanguage = "Enter the name of file with original language: ";
    const char* sentenceForLanguageOnRussian = "Enter the name of file where i will save language on Russian: ";

    char* fileWithOriginalLanguage = getNameOfFile( sentenceForOriginalLanguage );
    char* fileWithLanguageOnRussian = getNameOfFile( sentenceForLanguageOnRussian );

    translateLanguageOnRussian( fileWithOriginalLanguage, fileWithLanguageOnRussian );

    free( fileWithOriginalLanguage );
    free( fileWithLanguageOnRussian );

    return 0;
}
