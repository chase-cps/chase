/*
 * Grammar for Chase input.
 */

grammar Chase;

WS: [ \t\r\n]+ -> skip;

ENDST: ';';

fragment
    DIGIT:  [0-9];

fragment
    LETTER: [a-zA-Z_];

fragment
    ALNUM:  DIGIT | LETTER | '-';

ID:   LETTER ALNUM*;

NUMBER: DIGIT+;

LINE_COMMENT: '#' ~[\\\r\n]* '\r'? '\n' -> skip;

chaseSpec: types components abbreviations? connections requirements assumptions? EOF;

types: typesKW type+;

type: nam=name ':' typ=ID  ('(' par+=ID (',' par+=ID)* ')')? ENDST;

components: componentsKW component+;

component: nam=name ':' (num=NUMBER | '*') ENDST;

name: comp+=ID+ (num=NUMBER)? | num=NUMBER comp+=ID+;

abbreviations: abbreviationsKW abbrev+;

abbrev: nam=name ':' abbr=ID ENDST;

connections: connectionsKW conn* ((switched | unswitched) conn+)*;

switched:  switchedKW '(' sw=name ')';

unswitched: unswitchedKW;

conn: from=name ('--' | '-' '(' sw=name ')' '-') to=name ENDST;

requirements: requirementsKW req+;

req: pred=ID '(' par+=param ( ',' par+=param)* ')' ENDST;

param: nam=name | num=NUMBER;

assumptions: assumptionsKW assum+;

assum: req;

typesKW: 'TYPES';
componentsKW: 'COMPONENTS';
abbreviationsKW: 'ABBREVIATIONS' ;
requirementsKW: 'REQUIREMENTS' ;
connectionsKW: 'CONNECTIONS' ;
assumptionsKW: 'ASSUMPTIONS';
switchedKW: 'SWITCHED';
unswitchedKW: 'UNSWITCHED';
