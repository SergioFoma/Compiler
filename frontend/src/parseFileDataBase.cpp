#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "parseFileDataBase.h"
#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "mathOperatorsInfo.h"
#include "mathematicalCalculator.h"
#include "../../commonFiles/config.h"   // first - enter from src, second - enter from fronted

const size_t startSizeForWord = 7;
const size_t startSizeForTokens = 20;

#define COMMENT_BEGIN   "##!"
#define COMMENT_END     "!##"
#define COMMENT_LEN     strlen( COMMENT_BEGIN )

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile ){
    assert( bufferFromFile );
    assert( nameOfFile );

    if( myFile == NULL ){
        return fileOpenErr;
    }

    getFileSize( bufferFromFile, nameOfFile );
    bufferFromFile->buffer = (char*)calloc( bufferFromFile->fileSize + 1, sizeof( char ) );
    if( bufferFromFile->buffer == NULL ){
        return memoryErr;
    }
    bufferFromFile->bufferSize = fread( bufferFromFile->buffer, sizeof( char ), bufferFromFile->fileSize, myFile );
    if( bufferFromFile->bufferSize == 0 ){
        return fileOpenErr;
    }

    (bufferFromFile->buffer)[ bufferFromFile->fileSize ]= '\0';
    (bufferFromFile->buffer)[ bufferFromFile->bufferSize ] = '\0';

    return correct;
}

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile ){
    assert( bufferFromFile != NULL );
    assert( nameOfFile != NULL );

    struct stat fileText;
    int status = stat( nameOfFile, &fileText );
    if( status == -1 ){
        colorPrintf( NOMODE, RED, "\nError of get information from file\n" );
        return ;
    }

    bufferFromFile->fileSize = fileText.st_size;
}

void destroyBufferInformation( bufferInformation *bufferFromFile ){
    if( bufferFromFile == NULL ){
        return ;
    }

    free( bufferFromFile->buffer );
    bufferFromFile->bufferSize = 0;
    bufferFromFile->fileSize = 0;
    bufferFromFile = NULL;
}

bufferInformation getBufferFromFile( FILE** fileWithBuffer ){

    bufferInformation dataBaseFromFile = {};
    char* nameOfFile = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFile, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        free( nameOfFile );
        return dataBaseFromFile;
    }

    FILE* fileForReading = fopen( nameOfFile, "r" );
    if( fileForReading == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        free( nameOfFile );
        return dataBaseFromFile;
    }

    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForReading, nameOfFile );
    if( statusOfReadFromFile != correct ){
        free( nameOfFile );
        return dataBaseFromFile;
    }

    *fileWithBuffer = fileForReading;
    free( nameOfFile );
    return dataBaseFromFile;
}

void initializationTokens( infoForCreateTree* infoForTree ){
    infoForTree->countOfTokens = startSizeForTokens;
    infoForTree->tokens= ( node_t** )calloc( infoForTree->countOfTokens, sizeof( node_t* ) );
    size_t startIndex = 0;
    initializationNodeInArray( infoForTree, startIndex );

}

void initializationNodeInArray( infoForCreateTree* infoForTree, size_t startIndex ){
    assert( infoForTree );

    treeElem_t data = {};
    data.statement = NO_TYPE;
    for( ; startIndex < infoForTree->countOfTokens; startIndex++ ){
        initNode( infoForTree->tokens + startIndex, STATEMENT, data );
    }
}

void lexAnalysis( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    while( **symbol != '\0' ){
        if( infoForTree->freeIndexNow == ( infoForTree->countOfTokens - 1) ){
            infoForTree->countOfTokens *= 2;
            infoForTree->tokens = ( node_t** )realloc( infoForTree->tokens, sizeof( node_t* ) * infoForTree->countOfTokens );
            initializationNodeInArray( infoForTree, infoForTree->freeIndexNow + 1 );
        }

        checkingOnComment( symbol );

        if( tryInitializationExpressionNode( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationStatement( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationMathOperator( symbol, infoForTree ) == true ){
            continue;
        }


        if( tryInitializationNumber( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationVariable( symbol, infoForTree ) == true ){
            continue;

        }

        if( isspace( **symbol ) ){
            cleanLineWithCode( symbol );
            continue;
        }

        colorPrintf( NOMODE, RED, "\n\nERROR OF LEX ANALYSIS\n\nNOT UNDEFINED SYMBOL  '%c' \n",**symbol );
        exit( 0 );
    }
}

void checkingOnComment( char** symbol ){
    assert( symbol );
    assert( *symbol );

    if( strncmp( *symbol, COMMENT_BEGIN, COMMENT_LEN ) == 0 ){
        *symbol += COMMENT_LEN;
        while( strncmp( *symbol, COMMENT_END, COMMENT_LEN ) != 0 ){
            ++( *symbol );
        }
        *symbol += COMMENT_LEN;
    }
}

bool tryInitializationExpressionNode( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        const char* nameOfOperator = arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile;
        size_t lenOfOperator = strlen( arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile );

        if( strncmp( *symbol, nameOfOperator, lenOfOperator ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = EXPRESSION;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.expressionOperator = arrayWithExpressions[ expressionIndex ].expressionOperator;
            (*symbol) += lenOfOperator;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationStatement( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        const char* nameOfStatement = arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        size_t lenOfStatement = strlen( arrayWithStatements[ statementIndex ].viewOfStatementInFile );

        if( strncmp( *symbol, nameOfStatement, lenOfStatement ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = STATEMENT;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.statement = arrayWithStatements[ statementIndex ].statement;
            (*symbol) += lenOfStatement;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationMathOperator( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t mathIndex = 0; mathIndex < sizeOfStatementArray; mathIndex++ ){
        const char* nameOfMath = arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile;
        size_t lenOfMath = strlen( arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile );

        if( strncmp( *symbol, nameOfMath, lenOfMath ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = OPERATOR;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.mathOperation = arrayWithMathInfo[ mathIndex ].mathOperation;
            (*symbol) += lenOfMath;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationNumber( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    if( '0' <= **symbol && **symbol <= '9' ){
        double value = 0;
        do{
            value = value * 10 + ( **symbol - '0' );
            ++(*symbol);
        }while( '0' <= **symbol && **symbol <= '9' );

        (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = NUMBER;
        (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.number = value;
        ++( infoForTree->freeIndexNow );

        return true;
    }

    return false;
}

bool tryInitializationVariable( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    if( isalpha( **symbol ) || **symbol == '_' ){
        char* lineWithVar = NULL;
        size_t lineLen = readingVariable( &lineWithVar, symbol );
        bool statusOfSearching = changTypeOfNodeOnVariableNode( infoForTree, symbol, lineWithVar, lineLen );
        if( !statusOfSearching ){
            free( ( infoForTree->tokens )[ infoForTree->freeIndexNow ] );
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ] = makeNodeWithNewVariable( lineWithVar, symbol, lineLen, infoForVarArray.freeIndexNow );
        }
        ++( infoForTree->freeIndexNow );

        return true;
    }

    return false;
}

bool changTypeOfNodeOnVariableNode( infoForCreateTree* infoForTree, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen ){
    assert( infoForTree );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->nodeValueType = VARIABLE;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->data = data;
            *ptrOnSymbolInPosition += lineLen;
            free( lineWithVar );
            return true;
        }
    }
    return false;
}

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree ){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will find mathematical statement: " );

    FILE* fileForMathStatement = NULL;

    bufferInformation dataBaseFromFile = getBufferFromFile( &fileForMathStatement );
    char* ptrOnSymbolInPosition = dataBaseFromFile.buffer;

    infoForCreateTree infoForTree = {};
    initializationTokens( &infoForTree );

    lexAnalysis( &ptrOnSymbolInPosition, &infoForTree );
    infoForTree.currentIndex = 0;                               // start position - zero index

    printf( "\n freeIndex = %lu\ncurrentIndex = %lu\n", infoForTree.freeIndexNow, infoForTree.currentIndex  );
    tree->rootTree = getGeneral( &infoForTree );
    colorPrintf( NOMODE, GREEN, "\nSuccessfully reading an expression from a file\n");
    dumpLexArrayInFile( &infoForTree );

    destroyLexArray( &infoForTree );
    destroyBufferInformation( &dataBaseFromFile );
    fclose( fileForMathStatement );
    return CORRECT_WORK;
}

node_t* getGeneral( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = makeTreeFromOperators( infoForTree );

    printf( "token stoped = %lu\nsize of array = %lu", infoForTree->currentIndex, infoForTree->countOfTokens );
    return nodeOperator;
}

node_t* makeTreeFromOperators( infoForCreateTree* infoForTree ){
    if( infoForTree->currentIndex >= ( infoForTree->freeIndexNow - 1 ) ){
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    if( nodeOperator->right == NULL ){
        nodeOperator->right = makeTreeFromOperators( infoForTree );
    }
    else{
        nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, NULL );
        nodeOperator->right = makeTreeFromOperators( infoForTree );
    }

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}

node_t* getOperator( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, NULL, NULL );

    if( nodeOperator->left = getAssignment( infoForTree )  ){}
    else if( nodeOperator->left = getCondition( infoForTree ) ){}
    else if( nodeOperator->left = getCycle( infoForTree ) ){}
    else if( nodeOperator->left = getPrint( infoForTree ) ){}
    else if( nodeOperator->left = getInput( infoForTree ) ){}
    else if( nodeOperator->left = getFunctionDefinition( infoForTree ) ){}
    else if( nodeOperator->left = getFunctionDeclaration( infoForTree ) ){}
    else if( nodeOperator->left = getPrimaryExpression( infoForTree ) ){}
    else{

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

        destroyNode( nodeOperator );
        nodeOperator = getNodeFromOperatorThatCalledOperator( infoForTree );
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == OPERATOR_END ){
        ++( infoForTree->currentIndex );
    }

    return nodeOperator;
}

node_t* getNodeFromOperatorThatCalledOperator( infoForCreateTree* infoForTree ){
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    nodeOperator->right = getNodeFromOperatorThatCalledOperator( infoForTree );

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}

node_t* getCondition( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == IF &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == IF ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }

            node_t* left = getLogicalOr( infoForTree );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }

            node_t* right = getElse( infoForTree );
            return newStatementNode( STATEMENT, IF, left, right );
          }
    }

    return NULL;
}

node_t* getElse( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getOperator( infoForTree );
    node_t* right = NULL;

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == ELSE ){

        ++( infoForTree->currentIndex );

        right = getOperator( infoForTree );
    }

    return newStatementNode( STATEMENT, ELSE, left, right );

}

node_t* getCycle( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == WHILE &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == WHILE ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }

            node_t* left = getLogicalOr( infoForTree );

            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }

            node_t* right = getElse( infoForTree );
            return newStatementNode( STATEMENT, WHILE, left, right );
          }
    }

    return NULL;

}

node_t* getAssignment( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = NULL;
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == VARIABLE ){
        left = getVariable( infoForTree );
    }
    else{
        return NULL;
    }

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == ASSIGNMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == ASSIGNMENT ){

            ++( infoForTree->currentIndex );

            node_t* right = getLogicalOr( infoForTree );

            arrayWithVariableValue[ left->data.variableIndexInArray ] = calculateValue( right );
            return newStatementNode( STATEMENT, ASSIGNMENT, left, right );
        }
    }

    destroyNode( left );
    --( infoForTree->currentIndex );
    return NULL;
}

node_t* getFunctionDefinition( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == DEF ) ){

            return NULL;
    }

    ++( infoForTree->currentIndex );

    node_t* nameOfFunction = getVariable( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

    node_t* functionArgument = getArgumentInFunction( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
        ++( infoForTree->currentIndex );
    }

    node_t* left = newStatementNode( STATEMENT, OPERATOR_END, nameOfFunction, functionArgument );
    node_t* right = getOperator( infoForTree );

    return newStatementNode( STATEMENT, FUNC, left, right );
}

node_t* getFunctionDeclaration( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == VARIABLE ) ){
        return NULL;
    }

    node_t* nameOfFunction = getVariable( infoForTree );

    if( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ) ){

        destroyNode( nameOfFunction );
        --( infoForTree->currentIndex );
        return NULL;
    }

    ++( infoForTree->currentIndex );                                                             // skipping the opening par
    node_t* functionArgument = getArgumentInFunction( infoForTree );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
        ++( infoForTree->currentIndex );
    }

    return newStatementNode( STATEMENT, FUNC, nameOfFunction, functionArgument );
}

node_t* getPrint( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* printNode = NULL;

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PRINT ){

        ++( infoForTree->currentIndex );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

        printNode = getArgumentInFunction( infoForTree );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }

        printNode = newStatementNode( STATEMENT, PRINT, printNode, NULL );
    }

    return printNode;
}

node_t* getArgumentInFunction( infoForCreateTree* infoForTree ){
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
        return NULL;
    }

    node_t* nodeOperator = getOperator( infoForTree );
    nodeOperator->right = getArgumentInFunction( infoForTree );

    if( nodeOperator->left ){
        nodeOperator->left->parent = nodeOperator;
    }
    if( nodeOperator->right ){
        nodeOperator->right->parent = nodeOperator;
    }

    return nodeOperator;
}


node_t* getInput( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == INPUT ) {
        ++( infoForTree->currentIndex );
    }
    else{
        return NULL;
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ) {
        ++( infoForTree->currentIndex );
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ) {
        ++( infoForTree->currentIndex );
    }

    return newStatementNode( STATEMENT, INPUT, NULL, NULL );
}

const char* getEndOfAssignment(){
    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == OPERATOR_END ){
            return arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        }
    }

    return NULL;
}

node_t* getLogicalOr( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getLogicalAnd( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == OR ) ){

        ++( infoForTree->currentIndex );

        node_t* right = getLogicalAnd( infoForTree );

        left = newExpressionNode( EXPRESSION, OR, left, right );
    }

    return left;
}

node_t* getLogicalAnd( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getTypeOfEqual( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == AND ) ){

        ++( infoForTree->currentIndex );

        node_t* right = getTypeOfEqual( infoForTree );

        left = newExpressionNode( EXPRESSION, AND, left, right );
    }

    return left;
}

node_t* getTypeOfEqual( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getCompareOperations( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == EQUAL ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == NOT_EQUAL ) ) {

        typeOfExpressions currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;
        ++( infoForTree->currentIndex );

        node_t* right = getCompareOperations( infoForTree );

        left = newExpressionNode( EXPRESSION, currentExpression, left, right );
    }

    return left;
}

node_t* getCompareOperations( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getExpression( infoForTree );
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType != EXPRESSION ){
        return left;
    }

    typeOfExpressions currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
           ( currentExpression == BELOW || currentExpression == BELOW_OR_EQUAL || currentExpression == ABOVE || currentExpression == ABOVE_OR_EQUAL ) ) {

        ++( infoForTree->currentIndex );

        node_t* right = getExpression( infoForTree );

        left = newExpressionNode( EXPRESSION, currentExpression, left, right );
        currentExpression = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator;
    }

    return left;
}


node_t* getExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getTerm( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == ADD ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == SUB ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getTerm( infoForTree );

        if( currentMathOp == ADD ){
            left = newMathNode( OPERATOR, ADD, left, right );
        }
        else{
            left = newMathNode( OPERATOR, SUB, left, right );
        }
    }

    return left;
}

node_t* getTerm( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getPow( infoForTree );


    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == MUL ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == DIV ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getPow( infoForTree );

        if( currentMathOp == MUL ){
            left = newMathNode( OPERATOR, MUL, left, right );
        }
        else if( currentMathOp == DIV ){
            left = newMathNode( OPERATOR, DIV, left, right );
        }
    }

    return left;
}

node_t* getPow( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getPrimaryExpression( infoForTree );


    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == POW ){

        ++( infoForTree->currentIndex );

        node_t* right = getPrimaryExpression( infoForTree );

        left = newMathNode( OPERATOR, POW, left, right );
    }

    return left;

}

node_t* getPrimaryExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
        ++( infoForTree->currentIndex );

        node_t* nodeFromExpression = getLogicalOr( infoForTree );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }
        return nodeFromExpression;
    }

    node_t* currentNode = ( infoForTree->tokens )[ infoForTree->currentIndex ];
    node_t* resultNode = NULL;

    if( resultNode = getInput( infoForTree ) ){}
    else if( resultNode = getFunctionDeclaration( infoForTree ) ){}
    else if( currentNode->nodeValueType != NUMBER ){
        return getVariable( infoForTree );
    }
    else{
        return getNumber( infoForTree );
    }

    return resultNode;
}

node_t* getFunctionWithOneArgument( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }
            node_t* funcNode = getExpression( infoForTree );

            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }
            return newMathNode( OPERATOR, arrayWithMathInfo[ indexOfMathFunc ].mathOperation, NULL, funcNode );
        }
    }

    return NULL;
}

node_t* getVariable( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    bool isFunction = checkingOnFunction( infoForTree );
    if( isFunction ){
        return getFunctionWithOneArgument( infoForTree );
    }

    bool isStatement = checkingOnStatement( infoForTree );
    if( isStatement ){
        return NULL;
    }

    bool isExpression = checkingOnExpression( infoForTree );
    if( isExpression ){
        return getLogicalOr( infoForTree );
    }

    size_t treeIndex = ( infoForTree->currentIndex );
    ++( infoForTree->currentIndex );
    return copyNode( ( infoForTree->tokens )[ treeIndex ] );
}

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex ){
    assert( lineWithVar );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( infoForVarArray.freeIndexNow == infoForVarArray.capacity - 1){
            infoForVarArray.capacity *= 2;
            arrayWithVariables = (informationWithVariables*)realloc( arrayWithVariables, infoForVarArray.capacity * sizeof( informationWithVariables ) );
            arrayWithVariableValue = (double*)realloc( arrayWithVariableValue, infoForVarArray.capacity * sizeof( double ) );
        }

    arrayWithVariables[ infoForVarArray.freeIndexNow ] = { lineWithVar , infoForVarArray.freeIndexNow  };
    treeElem_t data = {};
    data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
    node_t* nodeWithVar = NULL;
    initNode( &nodeWithVar, VARIABLE, data );
    ++(infoForVarArray.freeIndexNow);
    *ptrOnSymbolInPosition += lineLen;

    arrayWithVariableValue[  infoForVarArray.freeIndexNow ] = 0;        // initialization
    cleanLineWithCode( ptrOnSymbolInPosition );
    return nodeWithVar;
}

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition ){
    assert( lineWithWord );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithWord = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( isalpha( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ){
            isEnoughSize( lineWithWord, &lineIndex, &sizeOfLine );
            (*lineWithWord)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithWord)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;
}

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition ){
    assert( lineWithVariable );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithVariable = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( isalpha( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ||
          ( '0' <= (*ptrOnSymbolInPosition)[ lineIndex ]  && (*ptrOnSymbolInPosition)[ lineIndex ]  <= '9')  ){
            isEnoughSize( lineWithVariable, &lineIndex, &sizeOfLine );
            (*lineWithVariable)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithVariable)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;

}

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine ){
    assert( lineWithWord );
    assert( lineIndex );
    assert( sizeOfLine );

    if( *lineIndex == *sizeOfLine - 1 ){
        *sizeOfLine *= 2;
        *lineWithWord = (char*)realloc( *lineWithWord, *sizeOfLine * sizeof( char ) );
    }
};

bool checkingOnFunction( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){
            return true;
        }
    }

    return false;
}

bool checkingOnStatement( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == arrayWithStatements[ statementIndex ].statement ){

                return true;
            }
    }

    return false;
}

bool checkingOnExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == EXPRESSION &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){

                return true;
            }
    }

    return false;
}

node_t* getNumber( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType == NUMBER ){
        size_t treeIndex = infoForTree->currentIndex;
        ++( infoForTree->currentIndex );
        return copyNode( (infoForTree->tokens)[ treeIndex ] );
    }

    colorPrintf( NOMODE, RED, "ERROR:%s %s %d\n typeOfNode = %d, current index = %lu\n", __FILE__, __func__, __LINE__,
                (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType, infoForTree->currentIndex );
    return NULL;
}

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
}


void dumpLexArrayInFile( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save info about tokens: " );

    char* nameOfFileForTokens = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForTokens, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ;
    }

    FILE* fileForTokens = fopen( nameOfFileForTokens, "w" );
    fprintf( fileForTokens, "array with tokens[ %lu ] = {\n", infoForTree->countOfTokens );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){
        if( ( infoForTree->tokens )[ tokenIndex ] == NULL ){
            continue;
        }

        if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == NUMBER ){
            fprintf( fileForTokens, "\t[%lu] = { type = NUMBER, val = %lg}\n", tokenIndex, ( infoForTree->tokens )[ tokenIndex ]->data.number );
        }
        else{
            fprintf( fileForTokens, "\t[%lu] = { type = %s, val = '%s'}\n",
                     tokenIndex, getStringOfValueType( ( infoForTree->tokens )[ tokenIndex] ), getStringOfNodeValue( ( infoForTree->tokens )[ tokenIndex] ) );
        }
    }
    fprintf( fileForTokens, "};\ncapacity = %lu\nfreeIndexNow = %lu\ncurrentIndex = %lu\n", infoForTree->countOfTokens, infoForTree->freeIndexNow, infoForTree->currentIndex );


    free( nameOfFileForTokens );
    fclose( fileForTokens );
}

void destroyLexArray( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){

        free( ( infoForTree->tokens )[tokenIndex ] );
    }

    free( infoForTree->tokens );
}
