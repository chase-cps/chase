/*
 * Grammar for the Contract and Component Design Language
 */
grammar CoCoDe_tokens;


WS: [ \t\r\n]+ -> skip;

LINE_COMMENT: '//'  ~[\\\r\n]* '\r'? '\n' -> skip;
MULTILINE_COMMENT: '/*' .*? '*/' -> skip;

SEMICOLON : ';';

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



/**
* Temporal Operators
**/

ALWAYS:     'G';
EVENTUALLY: 'F';
NEXT:       'X';
UNTIL:      'U';
RELEASE:    'R';

/*
* Relation operators
*/

EQ:     '=';
NEQ:    '!=';
LT:     '<';
LE:     '<=';
GT:     '>';
GE:     '>=';



/**
   Mathematical Operators
**/

PLUS:   '+';
MINUS:  '-';
TIMES:  '*';
DIVIDE: '/';

LROUND: '(';
RROUND: ')';

LSQUARE:    '[';
RSQUARE:    ']';

LCURLY:     '{';
RCURLY:     '}';

COMMA:  ',';
DOT:    '.';

ENDST: ';';