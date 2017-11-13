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
 * @file	chase/src/Behaviors/Logics/GR1Formula.cc
 * @brief
 */



#include "Behaviors/Logics/GR1Formula.hh"

using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Manipulation;

GR1Formula::GR1Formula( Behaviors::Logics::WellFormedFormula * f ) :
    _original(f),
    _progress(NULL),
    _safety(NULL),
    _initial(NULL)
{
    _createGR1Spec();
}

GR1Formula::~GR1Formula()
{
}

WellFormedFormula * GR1Formula::getOriginal()
{
    return _original;
}

WellFormedFormula * GR1Formula::getProgress()
{
    return _progress;
}

WellFormedFormula * GR1Formula::getSafety()
{
    return _safety;
}

WellFormedFormula * GR1Formula::getInitial()
{
    return _initial;
}

void GR1Formula::setOriginal( WellFormedFormula * f )
{
    _original = f;
}

void GR1Formula::setProgress( WellFormedFormula * f )
{
    _progress = f;
}

void GR1Formula::setSafety( WellFormedFormula * f )
{
    _safety = f;
}

void GR1Formula::setInitial( WellFormedFormula * f )
{
    _initial = f;
}

int GR1Formula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitGR1Formula(* this );
}

std::string GR1Formula::print()
{
    std::string s( _initial->print() );
    s += " &\n";
    s += _safety->print();
    s += " &\n";
    s += _progress->print();
    return s;
}

void GR1Formula::_createGR1Spec()
{

}
