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


ID: LETTER ALPHANUM*;
NUMBER: DIGIT+;

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
typeKW: integerKW | booleanKW;


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
    '(' formula ')' | atom;

minus_number: MINUS NUMBER;

value: value bin_math_op value |
        ID bin_math_op value |
        ID bin_math_op ID |
        value bin_math_op ID |
        ID | minus_number | NUMBER | '(' value ')';



atom:
    logic_constant | ID;



single_formula: formula ENDST;


assumptions:
    assumptionsKW ':'
    single_formula+;

guarantees:
    guaranteesKW ':'
    single_formula+;


declaration:
    (causality)? variableKW ID typeKW ENDST |
    constantKW ID integerKW NUMBER ENDST |
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



    
    













