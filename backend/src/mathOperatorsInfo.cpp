#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"

informationWithMathOperators arrayWithMathInfo[] = {
        { ADD   , "ADD"     , "+"       ,   TWO_ARG    },
        { SUB   , "SUB"     , "-"       ,   TWO_ARG    },
        { MUL   , "MUL"     , "*"       ,   TWO_ARG    },
        { DIV   , "DIV"     , "/"       ,   TWO_ARG    },
        { LN    , "LN"      , "ln"      ,   ONE_ARG    },
        { POW   , "POW"     , "^"       ,   TWO_ARG    },
        { SIN   , "SIN"     , "sin"     ,   ONE_ARG    },
        { COS   , "COS"     , "cos"     ,   ONE_ARG    },
        { TG    , "TG"      , "tg"      ,   ONE_ARG    },
        { CTG   , "CTG"     , "ctg"     ,   ONE_ARG    },
        { ARCSIN, "ARCSIN"  , "arcsin"  ,   ONE_ARG    },
        { ARCCOS, "ARCCOS"  , "arccos"  ,   ONE_ARG    },
        { ARCTG , "ARCTG"   , "arctg"   ,   ONE_ARG    },
        { ARCCTG, "ARCCTG"  , "arcctg"  ,   ONE_ARG    },
        { SH    , "SH"      , "sh"      ,   ONE_ARG    },
        { CH    , "CH"      , "ch"      ,   ONE_ARG    },
        { TH    , "TH"      , "th"      ,   ONE_ARG    },
        { CTH   , "CTH"     , "cth"     ,   ONE_ARG    },
        { EXP   , "EXP"     , "exp"     ,   ONE_ARG    },
        { SQRT  , "SQRT"    , "sqrt"    ,   ONE_ARG    }
};
const size_t sizeOfMathArray = sizeof( arrayWithMathInfo ) / sizeof( arrayWithMathInfo[ 0 ] );

informationWithStatements arrayWithStatements[] = {
        { NO_TYPE               ,   "NO_TYPE"       ,   "NO_TYPE"           ,   "NO_TYPE"       },
        { ASSIGNMENT            ,   "ASSIGNMENT"    ,   "pabgo_govnito"     ,   "="             },
        { OPERATOR_END          ,   "OP_END"        ,   "gusi"              ,   ";"             },
        { IF                    ,     "IF"          ,   "wool"              ,   "if"            },
        { PAR_OPEN              , "PAR_OPEN"        ,    "("                ,   "("             },
        { PAR_CLOSE             , "PAR_CLOSE"       ,    ")"                ,   ")"             },
        { CURLY_PAR_OPEN        , "CR_PAR_OPEND"    ,    "{"                ,   "{"             },
        { CURLY_PAR_CLOSE       , "CR_PAR_CLOSE"    ,    "}"                ,   "}"             },
        { WHILE                 , "WHILE"           ,    "beka"             ,   "while"         },
        { ELSE                  , "ELSE"            ,    "dandruff"         ,   "else"          }
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
