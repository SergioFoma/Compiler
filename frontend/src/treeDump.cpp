#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "treeDump.h"
#include "mathOperatorsInfo.h"
#include "../../commonFiles/config.h"   // first - enter from src, second - enter from fronted

static int firstRank = 1;

static void dumpTheSortedNodeInFile( const node_t* node, FILE* fileForWrite );

static void dumpTheSortedTreeInFile( const tree_t* tree, FILE* fileForWrite );

void dumpNode( node_t* node, int rank, FILE* treeFile ){
    assert( node );
    assert( treeFile );


    const char* stringNodeValueType = getStringOfValueType( node );
    if( node->nodeValueType == NUMBER ){
        fprintf( treeFile, "\tnode%lx [shape=\"plain\"; style =\"filled\"; fillcolor =\"hotpink2\"; label = "
                            "<<table><tr><td colspan = \"2\" > %p </td></tr> <tr><td colspan = \"2\" > parent = %p </td></tr> <tr><td width = \"100\" colspan = \"2\" > type = %s </td></tr> <tr><td width = \"100\" colspan = \"2\" > val = %lg </td></tr> ",
                            ( unsigned long )node, node, node->parent, stringNodeValueType, node->data.number  );
    }
    else{
        fprintf( treeFile, "\tnode%lx [shape=\"plain\"; style =\"filled\"; fillcolor =\"%s\"; label = "
                            "<<table><tr><td colspan = \"2\" > %p </td></tr> <tr><td colspan = \"2\" > parent = %p </td></tr> <tr><td width = \"100\" colspan = \"2\" > type = %s </td></tr> <tr><td width = \"100\" colspan = \"2\" > val = %s </td></tr> ",
                            ( unsigned long )node, getStringOfColor( node ) ,node, node->parent, stringNodeValueType, getStringOfNodeValue( node )  );
    }

    if( node->left ){
        fprintf( treeFile, "<tr><td> %p </td>", node->left );
    }
    else{
        fprintf( treeFile, "<tr><td> NULL </td>" );
    }

    if( node->right ){
        fprintf( treeFile, "<td> %p </td></tr>", node->right );
    }
    else{
        fprintf( treeFile, "<td> NULL </td></tr>" );
    }

    fprintf( treeFile, "</table>>;];\n" );

    fprintf( treeFile, "\t{\n"
                       "\t\trank = %d;\n"
                       "\t\tnode%lx;\n"
                       "\t}\n",
                       rank, ( unsigned long )node );

    if( node->left ){
        fprintf( treeFile, "\t\tnode%lx;\n"
                            "\tnode%lx->node%lx[color = \"orange\"];\n",
                            ( unsigned long )node->left, ( unsigned long )node, ( unsigned long )node->left );
        dumpNode( node->left, rank + 1, treeFile );
    }
    if( node->right ){
        fprintf( treeFile, "\t\tnode%lx;\n"
                           "\tnode%lx->node%lx[color = \"orange\"];\n",
                           ( unsigned long )node->right, ( unsigned long )node, ( unsigned long )node->right );
        dumpNode( node->right, rank + 1, treeFile );
    }

}

treeErrors dumpTree( tree_t* tree ){
    assert( tree );
    if( tree->rootTree == NULL ){
        return NODE_NULL_PTR;
    }

    FILE* treeFile = fopen( "frontend/treeDump.txt", "w" );

    if( treeFile == NULL ){
        return ERROR_OF_OPEN_FILE;
        fclose( treeFile );
    }

    fprintf( treeFile,  "digraph G{\n"
                        "\trankdir = TB;\n"
                        "\tnode[shape = \"hexagon\", color = \"black\", fontsize = 14, shape = record ];\n"
                        "\tedge[color = \"red\", fontsize = 12];\n" );

    dumpNode( tree->rootTree, firstRank, treeFile );

    fprintf( treeFile, "}" );

    fclose( treeFile );

    system("dot frontend/treeDump.txt -Tpng -o frontend/treeDump.png");

    //HTML

    FILE* htmlDump = fopen( "frontend/treeDump.html", "w" );
    if( htmlDump == NULL ){
        return ERROR_OF_OPEN_FILE;
    }

    fprintf( htmlDump, "<pre>\n"
                       "<h1> <font color=red> TREE DUMP </font>  </h1>"
                       "\n\n"
                       "<h1> TREE { %s:%s:%d } </h1>\n\n", __FILE__, __func__, __LINE__ );

    fprintf( htmlDump, "<h2 style = \"color: green;\">tree: \n");
    dumpTreeInFile( tree, htmlDump );


    fprintf( htmlDump, "</h2>\n\n<h2 style = \"color: orange;\">sorted tree: \n" );
    dumpTheSortedTreeInFile( tree, htmlDump );

    fprintf( htmlDump, "</h2>\n\n<h1>Image:</h1>\n"
                       "<img src=treeDump.png width = 2000px>\n\n");

    fclose( htmlDump );

    return CORRECT_TREE;
}

void dumpNodeInFile( const node_t* node, FILE* fileForPrint ){
    assert( node );
    assert( fileForPrint );

    if( node->nodeValueType == NUMBER ){
        fprintf( fileForPrint, "(%lg ", node->data.number );
    }
    else{
        fprintf( fileForPrint, "(%s ", getASTviewOfNodeValue( node ) );
    }

    if( node->left ){
        dumpNodeInFile( node->left, fileForPrint);
    }
    else{
        fprintf( fileForPrint, "nil " );
    }
    if( node->right ){
        dumpNodeInFile( node->right, fileForPrint );
    }
    else{
        fprintf( fileForPrint, "nil" );
    }

    fprintf( fileForPrint, ")" );
}

void dumpTreeInFile( const tree_t* tree, FILE* fileForPrint ){
    assert( tree );
    assert( fileForPrint );

    dumpNodeInFile( tree->rootTree, fileForPrint );
}

static void dumpTheSortedNodeInFile( const node_t* node, FILE* fileForPrint ){
    assert( node );
    assert( fileForPrint );

    if( node->left ){
        dumpTheSortedNodeInFile( node->left, fileForPrint);
    }

    if( node->nodeValueType == NUMBER ){
        fprintf( fileForPrint, " %lg ", node->data.number );
    }
    else if( node->nodeValueType == VARIABLE ){
        fprintf( fileForPrint, " %s ", getStringOfVariable( node ) );
    }
    else if( node->nodeValueType == OPERATOR ){
        fprintf( fileForPrint, " %s ", getViewOfMathOperation( node ) );
    }
    else if( node->nodeValueType == STATEMENT ){
        fprintf( fileForPrint, " %s ", getViewOfStatement( node ) );
    }
    else if( node->nodeValueType == EXPRESSION ){
        fprintf( fileForPrint, " %s ", getViewOfExpression( node ) );
    }

    if( node->right ){
        dumpTheSortedNodeInFile( node->right, fileForPrint);
    }
}

static void dumpTheSortedTreeInFile( const tree_t* tree, FILE* fileForPrint ){
    assert( tree );
    assert( fileForPrint );

    dumpTheSortedNodeInFile( tree->rootTree, fileForPrint );
}

const char* getStringOfValueType( const node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    for( size_t typeIndex = 0; typeIndex < sizeOfArrayWithValueType; typeIndex++ ){
        if( node->nodeValueType == arrayWithValueType[ typeIndex ].typeOfData ){
            return arrayWithValueType[ typeIndex ].nameOfType;
        }
    }

    return NULL;
}

const char* getStringOfColor( const node_t* node ){
    assert( node );

    switch( node->nodeValueType ){
        case OPERATOR:
            return "orange";
            break;
        case STATEMENT:
            return "slateblue1";
            break;
        case EXPRESSION:
            return "goldenrod1";
            break;
        case VARIABLE:
            return "lightskyblue2";
            break;
        default:
            return NULL;
            break;
    }

}

const char* getStringOfNodeValue( const node_t* node ){
    assert( node );

    switch( node->nodeValueType ){
        case OPERATOR:
            return getStringOfMathOperator( node );
            break;
        case STATEMENT:
            return getStringOfStatement( node );
            break;
        case EXPRESSION:
            return getStringOfExpression( node );
            break;
        case VARIABLE:
            return getStringOfVariable( node );
            break;
        default:
            return NULL;
            break;
    }

}

const char* getASTviewOfNodeValue( const node_t* node ){
    assert( node );

    switch( node->nodeValueType ){
        case OPERATOR:
            return getViewOfMathOperation( node );
            break;
        case STATEMENT:
            return getASTviewOfStatement( node );
            break;
        case EXPRESSION:
            return getASTviewOfExpression( node );
            break;
        case VARIABLE:
            return getStringOfVariable( node );
            break;
        default:
            return NULL;
            break;
    }

}

const char* getStringOfMathOperator( const node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    for( size_t mathIndex = 0; mathIndex < sizeOfMathArray; mathIndex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ mathIndex ].mathOperation ){
            return arrayWithMathInfo[ mathIndex ].nameOfMathOperation;
        }
    }

    return NULL;
}

const char* getStringOfVariable( const node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    for( size_t variableIndex = 0; variableIndex < infoForVarArray.freeIndexNow; variableIndex++ ){
        if( node->data.variableIndexInArray == arrayWithVariables[ variableIndex ].variableIndexInArray ){
            return arrayWithVariables[ variableIndex ].nameOfVariable;
        }
    }

    return NULL;
}

const char* getViewOfMathOperation( const node_t* node ){
    if( node == NULL ){
        return NULL;
    }

    for( size_t mathIndex = 0; mathIndex < sizeOfMathArray; mathIndex++ ){
        if( node->data.mathOperation ==  arrayWithMathInfo[ mathIndex ].mathOperation ){
            return arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile;
        }
    }

    return NULL;
}

const char* getStringOfStatement( const node_t* node ){
    assert( node );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( node->data.statement == arrayWithStatements[ statementIndex ].statement ){
            return arrayWithStatements[ statementIndex ].nameOfStatement;
        }
    }

    return NULL;
}

const char* getViewOfStatement( const node_t* node ){
    assert( node );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( node->data.statement == arrayWithStatements[ statementIndex ].statement ){
            return arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        }
    }

    return NULL;
}

const char* getASTviewOfStatement( const node_t* node ){
    assert( node );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( node->data.statement == arrayWithStatements[ statementIndex ].statement ){
            return arrayWithStatements[ statementIndex ].viewOfStatementInAST;
        }
    }

    return NULL;
}

const char* getStringOfExpression( const node_t* node ){
    assert( node );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( node->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){
            return arrayWithExpressions[ expressionIndex ].nameOfExpressions;
        }
    }

    return NULL;
}

const char* getViewOfExpression( const node_t* node ){
    assert( node );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( node->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){
            return arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile;
        }
    }

    return NULL;
}

const char* getASTviewOfExpression( const node_t* node ){
    assert( node );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        if( node->data.expressionOperator == arrayWithExpressions[ expressionIndex ].expressionOperator ){
            return arrayWithExpressions[ expressionIndex ].viewOfExpressionInAST;
        }
    }

    return NULL;
}
