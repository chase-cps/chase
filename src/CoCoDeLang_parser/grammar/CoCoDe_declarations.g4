/*
* This file contains all the rules to parse declarations.
* Variables, Constant, Parameters.
*/
grammar CoCoDe_declarations;

import CoCoDe_base;

basetype    :
    integerKW | realKW | booleanKW | complexKW;

/*
* Distributions.
*/

distribution_type :
    customKW | gaussianKW | homogeneousKW;

distribution_param  :
    ID EQ value;

distribution_params   :
    distribution_param (COMMA distribution_param)*;

vartype :
    basetype (inKW interval)?;

// The distribution may be either discrete or continous.
distribution_definition :
    distribution_type LROUND distribution_params RROUND
    (COMMA vartype)?;

distribution_declaration    :
    distributionKW ID;

distribution    :
    distribution_declaration isKW COLON distribution_definition ENDST;

distribution_instance : ID | distribution_definition;

/*
* Variables.
*/

controllability :
    controlledKW | uncontrolledKW;

variable    :
    controllability variableKW ID isKW COLON
        vartype
        (COMMA stochasticKW COLON distribution_instance)?
        (COMMA parametricKW)?
        ENDST |
    controllability vartype variableKW ID;

/*
* Constants.
*/
constant_definition :
    number | trueKW | falseKW
;

constant    :
    constantKW basetype ID COLON constant_definition ENDST;

declaration :
    constant | variable | distribution;

declarations :
    declaration*;