/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	frontend/src/Patterns2Chase/EPSPatterns/EPSPatterns.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/EPSPatterns.hh"
#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

using namespace Specification;
using namespace Specification::EPSPatterns;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;


// ******** PATTERN 1 ***********

std::vector< std::string > Pattern_1::_extractFormulae( std::string text )
{

    // Retrieve formulas
    std::string pattern(" starts with ");
    int pattern_length = pattern.length();

    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern_length;

    std::string formula_1st = text.substr(0, _begin);
    std::string formula_2nd = text.substr(_end );

    std::vector< std::string> ret;
    ret.push_back(formula_1st);
    ret.push_back(formula_2nd);

    return ret;
}

void Pattern_1::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{
    WellFormedFormula * formula_1 = formulae[0];
    LogicConstant * formula_2 = static_cast< LogicConstant * >( formulae[1] );

    /// \todo reverse formula if the value is false.
    //
    if( formula_2->getValue() == lc_true )
    {
    }
    else
    {
    }

    AorGVisitor visit( contract );
    formula_1->accept_visitor( visit );

    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, formula_1 );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, formula_1 );
        contract->assumptions = formula;

    }
}


// ******** PATTERN 2 ***********

std::vector<std::string> Pattern_2::_extractFormulae( std::string text )
{
    // Retrieve formulas
    std::string pattern("Always ");
    std::string formula = text.substr(pattern.length());

    std::vector< std::string> ret;
    ret.push_back(formula);

    return ret;
}

void Pattern_2::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{
    WellFormedFormula * formula_1 = formulae[0];
    UnaryTemporalFormula * nxt = 
        new UnaryTemporalFormula( op_next, formula_1 );
    UnaryTemporalFormula * global = 
        new UnaryTemporalFormula( op_globally, nxt );

    AorGVisitor visit( contract );
    global->accept_visitor( visit );

    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->assumptions = formula;
    }

}

// ******** PATTERN 3 ***********

std::vector<std::string> Pattern_3::_extractFormulae( std::string text )
{
    // Retrieve formulas
    std::string part_1st("if system sensed ");
    std::string part_2nd(" then do ");

    std::size_t end_1st = part_1st.length();
    std::size_t begin_2nd = text.find(part_2nd);
    std::size_t end_2nd = begin_2nd + part_2nd.length();

    std::string formula_1st = text.substr(end_1st, begin_2nd - end_1st);
    std::string formula_2nd = text.substr(end_2nd);

    std::vector< std::string> ret;
    ret.push_back(formula_1st);
    ret.push_back(formula_2nd);

    return ret;
}

void Pattern_3::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{
    WellFormedFormula * formula_1 = formulae[0];
    WellFormedFormula * formula_2 = formulae[1];

    UnaryTemporalFormula * nxt = new UnaryTemporalFormula( op_next, formula_2 );
    BinaryLogicCombination * imp = new BinaryLogicCombination(
            op_implies, formula_1, nxt);
    UnaryTemporalFormula * global = new UnaryTemporalFormula( op_globally, imp);

    AorGVisitor visit( contract );
    global->accept_visitor( visit );


    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->assumptions = formula;

    }

}

// ******** PATTERN 4 ***********

std::vector<std::string> Pattern_4::_extractFormulae( std::string text )
{
    // Retrieve formulas
    std::string part_1st("if system is sensing ");
    std::string part_2nd(" then do ");

    std::size_t end_1st = part_1st.length();
    std::size_t begin_2nd = text.find(part_2nd);
    std::size_t end_2nd = begin_2nd + part_2nd.length();

    std::string formula_1st = text.substr(end_1st, begin_2nd - end_1st);
    std::string formula_2nd = text.substr(end_2nd);

    std::vector< std::string> ret;
    ret.push_back(formula_1st);
    ret.push_back(formula_2nd);

    return ret;
}

void Pattern_4::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{
    WellFormedFormula * formula_1 = formulae[0];
    WellFormedFormula * formula_2 = formulae[1];

    UnaryTemporalFormula * nxt_1 = new UnaryTemporalFormula(
            op_next, formula_1 );
    UnaryTemporalFormula * nxt_2 = new UnaryTemporalFormula(
            op_next, formula_2 );

    BinaryLogicCombination * imp = new BinaryLogicCombination(
            op_implies, 
            nxt_1, 
            nxt_2 );

    UnaryTemporalFormula * global = new UnaryTemporalFormula(
            op_globally,
            imp );

    AorGVisitor visit( contract );
    global->accept_visitor( visit );


    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->assumptions = formula;

    }

}

// ******** PATTERN 5 ***********

std::vector<std::string> Pattern_5::_extractFormulae( std::string text )
{
    // Retrieve formulas
    std::string part_1st("if system activated ");
    std::string part_2nd(" and sensed ");
    std::string part_3rd(" then do ");

    std::size_t end_1st = part_1st.length();
    std::size_t begin_2nd = text.find(part_2nd);
    std::size_t end_2nd = text.find(part_2nd) + part_2nd.length();
    std::size_t begin_3rd = text.find(part_3rd);
    std::size_t end_3rd = begin_3rd + part_3rd.length();

    std::string formula_1st = text.substr(end_1st, begin_2nd - end_1st);
    std::string formula_2nd = text.substr(end_2nd, begin_3rd - end_2nd);
    std::string formula_3rd = text.substr(end_3rd);

    std::vector< std::string> ret;
    ret.push_back(formula_1st);
    ret.push_back(formula_2nd);
    ret.push_back(formula_3rd);

    return ret;
}

void Pattern_5::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{

    WellFormedFormula * formula_1 = formulae[0];
    WellFormedFormula * formula_2 = formulae[1];
    WellFormedFormula * formula_3 = formulae[2];

    BinaryLogicCombination * a = new BinaryLogicCombination(
            op_land,
            formula_1,
            formula_2 );

    UnaryTemporalFormula * nxt = new UnaryTemporalFormula(
            op_next,
            formula_3);

    BinaryLogicCombination * imp = new BinaryLogicCombination( 
            op_implies,
            a,
            nxt );

    UnaryTemporalFormula * global = new UnaryTemporalFormula(
            op_globally,
            imp );

    AorGVisitor visit( contract );
    global->accept_visitor( visit );

    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->assumptions = formula;

    }

}

// ******** PATTERN 6 ***********

std::vector<std::string> Pattern_6::_extractFormulae( std::string text )
{
    // Retrieve formulas
    std::string part_1st("do ");
    std::string part_2nd(" if and only if system is sensing ");

    std::size_t end_1st = part_1st.length();
    std::size_t begin_2nd = text.find(part_2nd);
    std::size_t end_2nd = begin_2nd + part_2nd.length();

    std::string formula_1st = text.substr(end_1st, begin_2nd - end_1st);
    std::string formula_2nd = text.substr(end_2nd);

    std::vector< std::string> ret;
    ret.push_back(formula_1st);
    ret.push_back(formula_2nd);

    return ret;
}


void Pattern_6::_createConstraints( 
        AGContract * contract,
        std::vector< WellFormedFormula * > formulae )
{
    WellFormedFormula * formula_1 = formulae[0];
    WellFormedFormula * formula_2 = formulae[1];

    BinaryLogicCombination * imp_1 = new BinaryLogicCombination(
            op_implies,
            formula_1,
            formula_2 );

    BinaryLogicCombination * imp_2 = new BinaryLogicCombination(
            op_implies,
            formula_2,
            formula_1 );

    BinaryLogicCombination * a = new BinaryLogicCombination(
            op_land,
            imp_1,
            imp_2 );

    UnaryTemporalFormula * nxt = new UnaryTemporalFormula(
            op_next,
            a );

    UnaryTemporalFormula * global = new UnaryTemporalFormula(
            op_globally,
            nxt );

    AorGVisitor visit( contract );
    global->accept_visitor( visit );

    bool guarantee = visit.getSysVarUsed();
    if( guarantee )
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getGuarantees());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->guarantees = formula;        
    }
    else
    {
        WellFormedFormula * wff = dynamic_cast< WellFormedFormula * >(
                contract->getAssumptions());
        if( wff == NULL )
        {
            exit(-1);
        }
        Behaviors::Logics::BinaryLogicCombination * formula =
            new BinaryLogicCombination( op_land, wff, global );
        contract->assumptions = formula;

    }

}



