/*
* Grammar to specify logistics problems.
*/

grammar LogisticsLang;

WS: [ \t\r\n]+ -> skip;

LINE_COMMENT: '#'  ~[\\\r\n]* '\r'? '\n' -> skip;

fragment DIGIT: [0-9];
fragment LETTER: [a-zA-Z];

fragment ALPHANUM: DIGIT | LETTER | '_';

COLON:      ':';
ENDST:      ';';

COMMA:  ',';
DOT:    '.';

LBRACKET: '(';
RBRACKET: ')';

// Maps

MAPCHAR: [fFrRlLdDuUBb@!><];

mapKw:  'Map';
MAPLINE : MAPCHAR MAPCHAR*;

map :
    mapKw LBRACKET MAPLINE+ RBRACKET;

// Products
DECIMAL: DOT DIGIT+;
NUMBER: DIGIT+ DECIMAL?;
ID: LETTER ALPHANUM*;

productsKw   : 'Products';

coordx      : NUMBER;
coordy      : NUMBER;
units       : NUMBER;
triple      : LBRACKET coordx COMMA coordy COMMA units RBRACKET;

product     : ID COLON triple (COMMA triple)* ENDST;

products    : productsKw LBRACKET product+ RBRACKET;

// Requirements

variableKw      : 'variable';
destinationKw   : 'destination';
time            : NUMBER | variableKw;
location        : ID;
request         : ID COLON NUMBER;

destination     :
    destinationKw location time LBRACKET
        request (COMMA request)* RBRACKET;

spec    : map products destination+;