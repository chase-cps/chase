/*
 * Grammar for the Contract and Component Design Language
 */
grammar CoCoDeLang;

import CoCoDe_declarations;

design_header :
    DesignKW isKW ID COLON;

design_body_element :
    declaration;

design :
    design_header
    design_body_element*;