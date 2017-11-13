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
 * @file	backend/include/backend/Chase2GR1C/GR1CPrinter.hh
 * @brief
 */
#include "Chase2GR1C/GR1CPrinter.hh"

using namespace chase;
using namespace backend;
using namespace Chase2GR1C;
using namespace GR1;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Contracts;


GR1CPrinter::GR1CPrinter( AGContract * contract, 
        std::string path,
        GR1Formula * assume,
        GR1Formula * guarantee ) :
    _contract( contract ),
    _assume(assume),
    _guarantee(guarantee),
    _file()
{
    _file.open( path.c_str(), std::ofstream::out );
}

GR1CPrinter::~GR1CPrinter()
{
    _file.close();
}

void GR1CPrinter::createGR1CFile()
{
    _printDeclarations();
    _printInit();
    _printSafe();
    _finalize();
}

void GR1CPrinter::_printHead()
{
    // Nothing to do here.
    /// @todo Find out if comments are possible in gr1c specifications.
}

void GR1CPrinter::_printDeclarations()
{
    _file << "ENV:";
    // Iterate over the variable declarations of the environment and print.

    std::list< Behaviors::Variable * >::iterator var_it;
    for( var_it = _contract->environment_variables.begin();
            var_it != _contract->environment_variables.end();
            ++var_it )
    {
        Behaviors::Variable * current = *var_it;
        std::string decl( _getVarDeclaration(current) );
        _file << std::endl << "\t" << decl;
    }
    _file <<";" << std::endl;
    _file << std::endl << std::endl;

    _file << "SYS:";
    for( var_it = _contract->system_variables.begin();
            var_it != _contract->system_variables.end();
            ++var_it )
    {
        Behaviors::Variable * current = *var_it;
        std::string decl( _getVarDeclaration(current) );
        _file << std::endl << "\t" << decl; 
    }
    _file <<";" << std::endl;
}


void GR1CPrinter::_printInit()
{
    InitialConditionsFinder afinder;
    _assume->accept_visitor( afinder );
    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    if( ! afinder.initial_conditions.empty() )
    {
        _file << "ENVINIT:";

        it = afinder.initial_conditions.begin();
        while(it != afinder.initial_conditions.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != afinder.initial_conditions.begin() )
            {
                _file << "& ";
            }
            _file << "(" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";

        _file << std::endl << std::endl;

    }


    InitialConditionsFinder gfinder;
    _guarantee->accept_visitor( gfinder );
    if( ! gfinder.initial_conditions.empty() )
    {
        _file << "SYSINIT:";
        it = gfinder.initial_conditions.begin();
        while(it != gfinder.initial_conditions.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != gfinder.initial_conditions.begin() )
            {
                _file << "& ";
            }
            _file << "(" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";
        _file << std::endl << std::endl;
    }
}

void GR1CPrinter::_printSafe()
{
    SafetyFinder afinder;
    _assume->accept_visitor( afinder );
    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    if( ! afinder.safety.empty() )
    {
        _file << "ENVTRANS:";

        it = afinder.safety.begin();
        while(it != afinder.safety.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != afinder.safety.begin() )
            {
                _file << "& ";
            }
            _file << " [](" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";
        _file << std::endl << std::endl;
    }

    SafetyFinder gfinder;
    _guarantee->accept_visitor( gfinder );
    if( ! gfinder.safety.empty() )
    {
        _file << "SYSTRANS:";
        it = gfinder.safety.begin();
        while(it != gfinder.safety.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != gfinder.safety.begin() )
            {
                _file << "& ";
            }
            _file << " [](" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";
        _file << std::endl << std::endl;
    }

}

void GR1CPrinter::_printProgress()
{
    ProgressFinder afinder;
    _assume->accept_visitor( afinder );
    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    if( ! afinder.progress.empty() )
    {
        _file << "ENVGOAL:";

        it = afinder.progress.begin();
        while(it != afinder.progress.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != afinder.progress.begin() )
            {
                _file << "& ";
            }
            _file << " [](" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";
        _file << std::endl << std::endl;
    }

    ProgressFinder gfinder;
    _guarantee->accept_visitor( gfinder );
    if( ! gfinder.progress.empty() )
    {
        _file << "SYSGOAL:";
        it = gfinder.progress.begin();
        while(it != gfinder.progress.end())
        {
            Behaviors::Logics::WellFormedFormula * current = *it;
            GR1CSyntaxWriter gr1cWriter;

            _file << std::endl << "\t";
            if( it != gfinder.progress.begin() )
            {
                _file << "& ";
            }
            _file << " [](" <<
                gr1cWriter.getGR1CRepresentation(current) << ")";

            ++it;
        }
        _file << ";";
        _file << std::endl << std::endl;
    }


}


void GR1CPrinter::_finalize()
{
    // Nothing to do here.
    /// @todo Find out if comments are possible in gr1c specifications.
}

std::string GR1CPrinter::_getVarDeclaration( Behaviors::Variable * var )
{
    std::string decl = var->GetName();

    if( var->GetVarType() == Behaviors::vartype::t_int )
    {
        decl += std::string(
                " [" + std::to_string((int) var->getMinValue()) +
                ", " + std::to_string((int) var->getMaxValue() - 2) +
                "]" );
    }
    return decl;
}

