#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"
#include "mathematicalCalculator.h"

informationWithMathOperators arrayWithMathInfo[] = {
        { ADD   , "ADD"     , "+"       ,   doAdd       ,   TWO_ARG    },
        { SUB   , "SUB"     , "-"       ,   doSub       ,   TWO_ARG    },
        { MUL   , "MUL"     , "*"       ,   doMul       ,   TWO_ARG    },
        { DIV   , "DIV"     , "/"       ,   doDiv       ,   TWO_ARG    },
        { LN    , "LN"      , "ln"      ,   doLn        ,   ONE_ARG    },
        { POW   , "POW"     , "^"       ,   doPow       ,   TWO_ARG    },
        { SIN   , "SIN"     , "sin"     ,   doSin       ,   ONE_ARG    },
        { COS   , "COS"     , "cos"     ,   doCos       ,   ONE_ARG    },
        { TG    , "TG"      , "tg"      ,   doTg        ,   ONE_ARG    },
        { CTG   , "CTG"     , "ctg"     ,   doCtg       ,   ONE_ARG    },
        { ARCSIN, "ARCSIN"  , "arcsin"  ,   doArcsin    ,   ONE_ARG    },
        { ARCCOS, "ARCCOS"  , "arccos"  ,   doArccos    ,   ONE_ARG    },
        { ARCTG , "ARCTG"   , "arctg"   ,   doArctg     ,   ONE_ARG    },
        { ARCCTG, "ARCCTG"  , "arcctg"  ,   doArcctg    ,   ONE_ARG    },
        { SH    , "SH"      , "sh"      ,   doSh        ,   ONE_ARG    },
        { CH    , "CH"      , "ch"      ,   doCh        ,   ONE_ARG    },
        { TH    , "TH"      , "th"      ,   doTh        ,   ONE_ARG    },
        { CTH   , "CTH"     , "cth"     ,   doCth       ,   ONE_ARG    },
        { EXP   , "EXP"     , "exp"     ,   doExp       ,   ONE_ARG    },
        { SQRT  , "SQRT"    , "sqrt"    ,   doSqrt      ,   ONE_ARG    }
};
const size_t sizeOfMathArray = sizeof( arrayWithMathInfo ) / sizeof( arrayWithMathInfo[ 0 ] );

informationWithStatements arrayWithStatements[] = {
        { NO_TYPE               ,   "NO_TYPE"       ,   "NO_TYPE"           ,   "NO_TYPE"       },
        { ASSIGNMENT            ,   "ASSIGNMENT"    ,   "pabgo_govnito"     ,   "="             },
        { OPERATOR_END          ,   "OP_END"        ,   "gusi"              ,   ";"             },
        { OPERATOR_END          ,   "OP_END"        ,    ","                ,   ";"             },
        { IF                    ,     "IF"          ,   "wool"              ,   "if"            },
        { PAR_OPEN              ,   "PAR_OPEN"      ,    "("                ,   "("             },
        { PAR_CLOSE             ,   "PAR_CLOSE"     ,    ")"                ,   ")"             },
        { CURLY_PAR_OPEN        ,   "CR_PAR_OPEND"  ,    "{"                ,   "{"             },
        { CURLY_PAR_CLOSE       ,   "CR_PAR_CLOSE"  ,    "}"                ,   "}"             },
        { WHILE                 ,   "WHILE"         ,    "beka"             ,   "while"         },
        { ELSE                  ,   "ELSE"          ,    "dandruff"         ,   "else"          },
        { PRINT                 ,   "PRINT"         ,    "give_info"        ,   "print"         },
        { INPUT                 ,   "INPUT"         ,    "get_info"         ,   "input"         },
};
const size_t sizeOfStatementArray = sizeof( arrayWithStatements ) / sizeof( arrayWithStatements[ 0 ] );

struct informationWithExpressions arrayWithExpressions[] = {
        { AND                   ,       "AND"           ,  "|BMP|"       ,       "&&"             },
        { OR                    ,       "OR"            ,  "|AIRDROP|"   ,       "||"             },
        { ABOVE_OR_EQUAL        ,       "ABOVE_OR_EQ"   ,  ">="          ,       ">="             },
        { BELOW_OR_EQUAL        ,       "BELOW_OR_EQ"   ,  "<="          ,       "<="             },
        { EQUAL                 ,       "EQUAL"         ,  "=="          ,       "=="             },
        { NOT_EQUAL             ,       "NOT_EQUAL"     ,  "!="          ,       "!="             },
        { BELOW                 ,       "BELOW"         ,  "<"           ,       "<"              },
        { ABOVE                 ,       "ABOVE"         ,  ">"           ,       ">"              }
};
const size_t sizeOfExpressionArray = sizeof( arrayWithExpressions ) / sizeof( arrayWithExpressions[ 0 ] );

size_t startIndexForArrayWithVar = 0;
informationWithVariables* arrayWithVariables = ( informationWithVariables* )calloc( startIndexForArrayWithVar + 1, sizeof( informationWithVariables ) );
informationWithVariablesArray infoForVarArray = { startIndexForArrayWithVar + 1, startIndexForArrayWithVar };

double* arrayWithVariableValue = (double*)calloc( startIndexForArrayWithVar + 1, sizeof( double ) );

informationWithValueType arrayWithValueType[] = {
        { NUMBER        ,     "NUMBER"        },
        { VARIABLE      ,     "VARIABLE"      },
        { OPERATOR      ,     "OPERATOR"      },
        { STATEMENT     ,     "STATEMENT"     },
        { EXPRESSION    ,     "EXPRESSION"    }
};
const size_t sizeOfArrayWithValueType = sizeof( arrayWithValueType ) / sizeof( arrayWithValueType[ 0 ] );
