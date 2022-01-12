/* 
 * Grammar to specify A/G Contracts using LTL.
 */

grammar LogicsContracts;

WS: [ \t\r\n]+ -> skip;

LINE_COMMENT: '#'  ~[\\\r\n]* '\r'? '\n' -> skip;

fragment DIGIT: [0-9];
fragment LETTER: [a-zA-Z];

fragment ALPHANUM: DIGIT | LETTER | '_';

/**
* Logic Operators
**/

AND:        '/\\' | '&' ;
OR:         '\\/' | '|' ;
NOT:        '!' | '~';
IMPLIES:    '->';
IFF:        '<->';

COLON:      ':';

bin_logic_op: AND | OR | IFF | IMPLIES;
unary_logic_op: NOT;

/**
* Temporal Operators
**/

ALWAYS:     'G';
EVENTUALLY: 'F';
NEXT:       'X';
UNTIL:      'U';
RELEASE:    'R';
PROBABILITY: 'P';

unary_temp_op: ALWAYS | EVENTUALLY | NEXT;
bin_temp_op: UNTIL | RELEASE;

/*
* Relation operators
*/

EQ:     '=';
NEQ:    '!=';
LT:     '<';
LE:     '<=';
GT:     '>';
GE:     '>=';

relation_op: EQ | NEQ | LT | LE | GT | GE;

/**
   Mathematical Operators
**/

PLUS:   '+';
MINUS:  '-';
TIMES:  '*';
DIVIDE: '/';

bin_math_op: TIMES | DIVIDE | PLUS | MINUS;

LBRACKET: '(';
RBRACKET: ')';

LSQUARE:    '[';
RSQUARE:    ']';

COMMA:  ',';
DOT:    '.';

ID: LETTER ALPHANUM*;
DECIMAL: DOT DIGIT+;
NUMBER: MINUS? DIGIT+ DECIMAL?;

ENDST: ';';

/*
* Keywords
*/

integerKW:      'integer';
booleanKW:      'boolean';
realKW:         'real';
variableKW:     'variable';
inputKW:        'input';
outputKW:       'output';
constantKW:     'constant';
propositionKw:  'proposition';
isKw:           'is';
trueKW:         'true';
falseKW:        'false';
contractKW:     'CONTRACT';
assumptionsKW:  'Assumptions';
guaranteesKW:   'Guarantees';

nameKw: 'NAME';

causality: inputKW | outputKW;
logic_constant: trueKW | falseKW;

range: LBRACKET NUMBER COLON NUMBER RBRACKET;
integer : integerKW range?;
real    : realKW range?;
typeKW: integer | real | booleanKW;

/**
  GRAMMAR RULES
**/

value: value bin_math_op value | primed_ID |
        ID | minus_ID | minus_number | NUMBER |
        PROBABILITY LBRACKET formula RBRACKET |
        LBRACKET value RBRACKET;

lvalue: value;
rvalue: value;

primed_ID : ID'\'';

relation:
    lvalue relation_op rvalue;

pair:
    value COMMA value;

interval_leftopen:
    RSQUARE pair RSQUARE;

interval_rightopen:
    LSQUARE pair LSQUARE;

interval_fullopen:
    RSQUARE pair LSQUARE;

interval_closed:
    LSQUARE pair RSQUARE;

interval:
    interval_closed |
    interval_fullopen |
    interval_leftopen |
    interval_rightopen;

formula:
    LBRACKET formula RBRACKET |
    unary_logic_op  formula |
    formula bin_logic_op formula |
    unary_temp_op (interval)? formula |
    formula bin_temp_op (interval)? formula |
    atom;

minus_number: MINUS NUMBER;
minus_ID: MINUS ID;


atom:
    logic_constant | relation | ID | primed_ID;

single_formula: formula ENDST;

assumptions:
    assumptionsKW ':'
    single_formula+;

guarantees:
    guaranteesKW ':'
    single_formula+;


declaration:
    (causality)? typeKW variableKW ID ENDST |
    typeKW constantKW ID  NUMBER ENDST |
    propositionKw ID (isKw relation)? ENDST ;

contract:
    contractKW ID ':'
    declaration*
    assumptions?
    guarantees?;

name: nameKw ':' ID ENDST;
    
systemSpec:
    name?
    declaration* contract+;

