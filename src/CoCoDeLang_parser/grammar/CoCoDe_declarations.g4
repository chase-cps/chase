/*
* This file contains all the rules to parse declarations.
* Variables, Constant, Parameters.
*/
grammar CoCoDe_declarations;

import CoCoDe_base;

basetype    :
    integerKW | realKW | booleanKW | complexKW;

domain  :
    interval | list;

/*
* Distributions.
*/

distribution_type   :
    discreteKW (domain)? | continuousKW (domain)?;

frequency_entry :
    value COLON number
;

frequency_table :
    LSQUARE frequency_entry+ RSQUARE
;

distribution_features   :
    gaussianKW LROUND (muKW EQ)? value COMMA (sigmaKW EQ)? value RROUND |
    homogeneousKW |
    frequenciesKW COLON frequency_table;

// The distribution may be either discrete or continous.
distribution_definition :
    distribution_type COMMA distribution_features;

distribution_declaration    :
    distributionKW ID;

distribution    :
    distribution_declaration isKW distribution_definition SEMICOLON;

distribution_instance : ID | distribution_definition;

/*
* Variables.
*/

controllability :
    controlledKW | uncontrolledKW;

variable    :
    controllability variableKW ID isKW COLON
        basetype (inKW domain)?
        (COMMA stochasticKW COLON distribution_instance)?
        (COMMA parametricKW COLON domain)
        SEMICOLON;

/*
* Constants.
*/
constant_definition :
    number | trueKW | falseKW
;

constant    :
    constantKW basetype ID COLON constant_definition SEMICOLON;

declaration :
    constant | variable | distribution;

declarations :
    declaration*;