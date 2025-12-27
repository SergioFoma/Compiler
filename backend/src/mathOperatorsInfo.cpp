#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"
#include "dataForASMfromBackend.h"

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
        { NO_TYPE               ,   "NO_TYPE"       ,   "NO_TYPE"           ,   "NO_TYPE"  ,      translateAssignmentInASM       },
        { ASSIGNMENT            ,   "ASSIGNMENT"    ,   "pabgo_govnito"     ,   "="        ,      translateAssignmentInASM       },
        { OPERATOR_END          ,   "OP_END"        ,   "gusi"              ,   ";"        ,      translateEndOperatorInASM      },
        { OPERATOR_END          ,   "OP_END"        ,    ","                ,   ";"        ,      translateEndOperatorInASM      },
        { IF                    ,     "IF"          ,   "wool"              ,   "if"       ,      translateIfInASM               },
        { PAR_OPEN              ,   "PAR_OPEN"      ,    "("                ,   "("        ,      translateAssignmentInASM       },
        { PAR_CLOSE             ,   "PAR_CLOSE"     ,    ")"                ,   ")"        ,      translateAssignmentInASM       },
        { CURLY_PAR_OPEN        ,   "CR_PAR_OPEND"  ,    "{"                ,   "{"        ,      translateAssignmentInASM       },
        { CURLY_PAR_CLOSE       ,   "CR_PAR_CLOSE"  ,    "}"                ,   "}"        ,      translateAssignmentInASM       },
        { WHILE                 ,   "WHILE"         ,    "beka"             ,   "while"    ,      translateWhileInASM            },
        { ELSE                  ,   "ELSE"          ,    "dandruff"         ,   "else"     ,      translateElseInASM             },
        { PRINT                 ,   "PRINT"         ,    "give_info"        ,   "print"    ,      translatePrintInASM            },
        { INPUT                 ,   "INPUT"         ,    "get_info"         ,   "input"    ,      translateInputInASM            },
};
const size_t sizeOfStatementArray = sizeof( arrayWithStatements ) / sizeof( arrayWithStatements[ 0 ] );

struct informationWithExpressions arrayWithExpressions[] = {
        { AND                   ,       "AND"           ,  "|BMP|"       ,       "&&"      ,      translateAboveOrEqualInASM      },
        { OR                    ,       "OR"            ,  "|AIRDROP|"   ,       "||"      ,      translateAboveOrEqualInASM      },
        { ABOVE_OR_EQUAL        ,       "ABOVE_OR_EQ"   ,  ">="          ,       ">="      ,      translateAboveOrEqualInASM      },
        { BELOW_OR_EQUAL        ,       "BELOW_OR_EQ"   ,  "<="          ,       "<="      ,      translateBelowOrEqualInASM      },
        { EQUAL                 ,       "EQUAL"         ,  "=="          ,       "=="      ,      translateEqualInASM             },
        { NOT_EQUAL             ,       "NOT_EQUAL"     ,  "!="          ,       "!="      ,      translateNoteEqualInASM         },
        { BELOW                 ,       "BELOW"         ,  "<"           ,       "<"       ,      translateBelowInASM             },
        { ABOVE                 ,       "ABOVE"         ,  ">"           ,       ">"       ,      translateAboveInASM             }
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
