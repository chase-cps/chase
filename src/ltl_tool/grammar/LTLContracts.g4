/* 
 * Grammar to specify A/G Contracts using LTL.
 */

grammar LTLContracts;

WS: [ \t\r\n]+ -> skip;

LINE_COMMENT: '#'  ~[\\\r\n]* '\r'? '\n' -> skip;

fragment DIGIT: [0-9];
fragment LETTER: [a-zA-Z];

fragment ALPHANUM: DIGIT | LETTER | '_';

NUMBER: DIGIT+;

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

ENDST: ';';

/*
* Keywords
*/

integerKW:      'integer';
booleanKW:      'boolean';
variableKW:     'variable';
constantKW:     'constant';
trueKW:         'true';
falseKW:        'false';
contractKW:     'CONTRACT';
assumptionsKW:  'Assumptions';
guaranteesKW:   'Guarantees';

nameKw: 'NAME';

logic_constant: trueKW | falseKW;
typeKW: integerKW | booleanKW;


/**
  GRAMMAR RULES
**/



relation:
    value relation_op value |
    '(' relation ')';


formula:
    unary_logic_op formula |
    formula bin_logic_op formula |
    unary_temp_op formula |
    formula bin_temp_op formula |
    '(' formula ')' | atom;

value: value bin_math_op value |
            '(' value ')' | ID | NUMBER;

atom:
    logic_constant | relation | ID;

single_formula:
    formula ENDST;

assumptions:
    assumptionsKW ':'
    single_formula+;

guarantees:
    guaranteesKW ':'
    single_formula+;


declaration:
    variableKW ID typeKW ENDST |
    constantKW ID integerKW NUMBER ENDST |
    constantKW ID booleanKW logic_constant ENDST ;

contract:
    contractKW ID ':'
    declaration*
    assumptions
    guarantees;

name: nameKw ':' ID ENDST;
    
systemSpec:
    name?
    declaration* contract+;
    
    













