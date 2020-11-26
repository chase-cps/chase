/* 
 * Grammar to specify A/G Contracts using LTL.
 */

grammar LTLContracts;

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

ALWAYS:     '[]';
EVENTUALLY: '<>';
NEXT:       'X';
UNTIL:      'U';



unary_temp_op: ALWAYS | EVENTUALLY | NEXT;
bin_temp_op: UNTIL;

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

ID: LETTER ALPHANUM*;
NUMBER: MINUS? DIGIT+;

ENDST: ';';

/*
* Keywords
*/

integerKW:      'integer';
booleanKW:      'boolean';
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
typeKW: integer | booleanKW;

/**
  GRAMMAR RULES
**/

lvalue: value;
rvalue: value;

relation:
    lvalue relation_op rvalue;

formula:
    unary_logic_op formula |
    formula bin_logic_op formula |
    unary_temp_op formula |
    formula bin_temp_op formula |
    LBRACKET formula RBRACKET | atom;

minus_number: MINUS NUMBER;
minus_ID: MINUS ID;

value: value bin_math_op value |
        ID bin_math_op value |
        ID bin_math_op ID |
        value bin_math_op ID |
        ID | minus_ID | minus_number | NUMBER | LBRACKET value RBRACKET;

atom:
    logic_constant | relation | ID;

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
