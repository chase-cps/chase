/*
* Grammar to specify logistics problems.
*/

grammar LogisticsLang;

WS: [ \t\r\n]+ -> skip;

LINE_COMMENT: '#'  ~[\\\r\n]* '\r'? '\n' -> skip;

fragment DIGIT: [0-9];
fragment LETTER: [a-zA-Z];

fragment ALPHANUM: DIGIT | LETTER | '_';

// Punctuation.
COLON:      ':';
COMMA:      ',';
DOT:        '.';
EQ:         '=';
ENDST:      ';';

LBRACKET: '(';
RBRACKET: ')';

// IDs and NUMBERs
ID: LETTER ALPHANUM*;
DECIMAL: DOT DIGIT+;

NUMBER: DIGIT+ DECIMAL?;

// Keywords
widgetsKw:      'Widgets';
componentsKw:   'Components';
crossroadsKw:   'Crossroads';
architectureKw: 'Architecture';
isKw:           'is';
requirementsKw: 'Requirements';

producesKw:     'produces';
requiresKw:     'requires';

binKw:          'Bin';
machineKw:      'Machine';
sinkKw:         'Sink';

nothingKw:         'Nothing';

// Widgets
widgets:
    widgetsKw COLON ID (COMMA ID)* ENDST;

/*
Components.
*/
item:
    NUMBER ID;

bin:
    ID isKw binKw LBRACKET item (COMMA item)* RBRACKET ENDST;

sink:
    ID isKw sinkKw LBRACKET item (COMMA item)* RBRACKET ENDST;

require: requiresKw COLON item (COMMA item)* | requiresKw COLON nothingKw;
produce: producesKw COLON item (COMMA item)* | producesKw COLON nothingKw;

machineSpec:
    produce ENDST require | require ENDST produce;

machine:
    ID isKw machineKw LBRACKET machineSpec RBRACKET ENDST;

component: machine | sink | bin;

components: componentsKw COLON component+;

/*
Crossroads.
*/

crossroad:
    ID COMMA NUMBER ENDST;

crossroads:
    crossroadsKw COLON crossroad+;

/*
Architecture
*/
ARROW_PRE: '-(';
ARROW_POST: ')->';

road:
    ARROW_PRE NUMBER ARROW_POST;

connection:
    ID road ID ENDST;

architecture:
    architectureKw COLON
        connection+;

/*
Requirements.
*/

requirement: NUMBER ID ENDST;

requirements :
    requirementsKw COLON requirement+;

spec:
    widgets components crossroads architecture requirements;