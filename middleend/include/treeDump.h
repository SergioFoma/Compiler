#ifndef H_TREEDUMP
#define H_TREEDUMP

#include "tree.h"

void dumpNode( node_t* node, int rank, FILE* treeFile );

const char* getStringOfValueType( const node_t* node );

const char* getStringOfColor( const node_t* node );

const char* getStringOfMathOperator( const node_t* node );

const char* getStringOfNodeValue( const node_t* node );

const char* getASTviewOfNodeValue( const node_t* node );

const char* getStringOfVariable( const node_t* node );

const char* getViewOfMathOperation( const node_t* node );

const char* getStringOfStatement( const node_t* node );

const char* getViewOfStatement( const node_t* node );

const char* getASTviewOfStatement( const node_t* node );

const char* getStringOfExpression( const node_t* node );

const char* getViewOfExpression( const node_t* node );

const char* getASTviewOfExpression( const node_t* node );

treeErrors dumpTree( tree_t* tree );

void dumpNodeInFile( const node_t* node, FILE* fileForWrite );

void dumpTreeInFile( const tree_t* tree, FILE* fileForWrite );

#endif
