grammar CoCoDe_base;
import CoCoDe_tokens;

bin_logic_op: AND | OR | IFF | IMPLIES;
unary_logic_op: NOT;

unary_temp_op:
    ALWAYS | EVENTUALLY | NEXT;
bin_temp_op:
    UNTIL | RELEASE;

relation_op:
    EQ | NEQ | LT | LE | GT | GE;

bin_math_op:
    TIMES | DIVIDE | PLUS | MINUS | POWER | MOD;

ID:
    LETTER ALPHANUM*;

INTEGER: DIGIT+;

DECIMAL:
    DOT DIGIT+;

number:
    MINUS? INTEGER DECIMAL?;

/*
* Keywords of the language.
*/

// Types
integerKW:      'Integer';
booleanKW:      'Boolean';
realKW:         'Real';
complexKW:      'Complex';

// Declarations - Contracts.
contractKW:     'Contract';
variableKW:     'variable';
constantKW:     'constant';

parametricKW:   'parametric';

controlledKW:   'controlled';
uncontrolledKW: 'uncontrolled';

isKW:           'is';
inKW:           'in';
stochasticKW :     'stochastic';
distributionKW  :   'distribution';


// Declarations - Components.


componentKW:    'Component';

inputKW:        'input';
outputKW:       'output';

propositionKW:  'proposition';
trueKW:         'true';
falseKW:        'false';
assumptionsKW:  'Assumptions';
guaranteesKW:   'Guarantees';
systemKW: 'System';

discreteKW      : 'discrete';
continuousKW    : 'continuous';
gaussianKW  : 'gaussian';
customKW    : 'custom';
muKW    : 'mu';
sigmaKW : 'sigma';
homogeneousKW   : 'homogeneous';
frequenciesKW   : 'frequencies';

DesignKW        : 'Design';

// Values.

primed_ID   : ID'\'';

minus_ID    : MINUS ID;

function : ID;

// Matrix
matrix_line : value (COMMA value)*;
matrix      : LSQUARE (matrix_line ENDST)* matrix_line RSQUARE;

value:
    value bin_math_op value | primed_ID |
    ID | minus_ID | number | LROUND value RROUND |
    function LROUND value (COMMA value)* RROUND |
    matrix |
    trueKW | falseKW;

// intervals.
pair  :     value COMMA value;

interval_leftopen   :   LROUND pair RSQUARE;
interval_rightopen  :   LSQUARE pair RROUND;
interval_fullopen   :   LROUND pair RROUND;
interval_closed     :   LSQUARE pair RSQUARE;

interval    :
    interval_closed |
    interval_fullopen |
    interval_leftopen |
    interval_rightopen;

// Lists of values.
list    :   LCURLY value (COMMA value)* RCURLY;




