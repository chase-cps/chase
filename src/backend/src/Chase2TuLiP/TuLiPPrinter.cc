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
 * @file	backend/src/backend/Chase2TuLiP/TuLiPPrinter.cc
 * @brief
 */



#include "Chase2TuLiP/TuLiPPrinter.hh"

using namespace chase;
using namespace backend;
using namespace Chase2TuLiP;
using namespace GR1;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Contracts;

TuLiPPrinter::TuLiPPrinter( AGContract * contract, 
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

TuLiPPrinter::~TuLiPPrinter()
{
    _file.close();
}

void TuLiPPrinter::createTuLiPFile()
{
    _printHead();
    _printDeclarations();
    _printInit();
    _printSafe();
    _printProgress();
    _finalize();
}

void TuLiPPrinter::_printHead()
{
    _file <<
        "from timeit import default_timer as timer" << std::endl <<
        "import argparse" << std::endl <<
        "import logging" << std::endl <<
        "import sys" << std::endl <<
        "import math" << std::endl <<
        "logging.basicConfig(level=logging.WARNING)" << std::endl <<
        "import tulip, Counterstrategies" << std::endl <<
        "from tulip import spec, synth, transys, dumpsmach" << std::endl <<

        "try:" << std::endl <<
        "\tfrom tulip.interfaces import gr1c" << std::endl <<
        "\tgr1c_available=True;" << std::endl <<
        "except ImportError, e:" << std::endl <<
        "\tgr1c_available=False;" << std::endl <<
        std::endl <<         
        "try:" << std::endl <<
        "\tfrom tulip.interfaces import omega " << std::endl <<
        "\tomega_available=True;" << std::endl <<
        "except ImportError, e:" << std::endl <<
        "\tomega_available=False;" << std::endl << 
        std::endl <<
        "if gr1c_available==True:" << std::endl <<
        "\tsolver='gr1c';" << std::endl <<
        "elif omega_available==True:" << std::endl <<
        "\tsolver='omega';" << std::endl <<
        "else:" << std::endl <<
        "\tsys.exit('ERROR: No solver available. Please check GR1C and Omega installation.');"
        << std::endl << std::endl;

       

    _file <<
        "start_time = timer()" << std::endl << std::endl;

    _file <<
        "parser = argparse.ArgumentParser(description=" <<
        "'Check realizability of the strategy.')" << std::endl <<
        "parser.add_argument('-g', '--graphycal', required=False," 
        << " action='store_true'," << std::endl <<
        "\tdest='graph'," << std::endl <<
        "\thelp='Print a graphical representation of the strategy, " 
        << " if any.')" << std::endl <<
        "parser.add_argument('-i', '--implement', required=False," 
        << " action='store_true'," << std::endl <<
        "\tdest='implement'," << std::endl <<
        "\thelp='Print the implementation of the strategy, " 
        << " if any.')" << std::endl <<
        "parser.add_argument('-p', '--python', required=False, " 
        << " default='automaton.py',"  << std::endl <<
        "\tdest='pythonfile'," << std::endl <<
        "\thelp='Specifies the file where to save the python " 
        << " implementation of the strategy, if any.')" << std::endl <<
        "parser.add_argument('-f', '--figure', required=False, " 
        << " default='automaton.eps'," << std::endl <<
        "\tdest='graphicfile'," << std::endl <<
        "\thelp='Specifies the file where to save the graphycal " 
        << " representation of the strategy, if any.')" << std::endl <<
         "parser.add_argument('-c', '--counterstrategy', required=False," 
        << " action='store_true'," << std::endl <<
        "\tdest='counterstrategy'," << std::endl <<
        "\thelp='Print a counterstrategy, " 
        << " if any.')" << std::endl <<     
        "parser.add_argument('-csp', '--counterstrategy_prefix', " << 
        "required=False,  default='/tmp/'," << std::endl <<
	    "\tdest='csprefix'," << std::endl <<
	    "\thelp='Specifies the prefix to use to store the " <<
        "counterstrategy, if any.')" << std::endl <<
        std::endl << std::endl <<
        "args = parser.parse_args()" << std::endl <<
        "pictorial_output = args.graphicfile" << std::endl <<
        "implementation_output = args.pythonfile" << std::endl << 
        "csprefix = args.csprefix" << std::endl << 
        std::endl;

}

void TuLiPPrinter::_printDeclarations()
{
    _file <<
        "# Declarations: Environment Variables" << std::endl << std::endl;
    _file << "env_vars = {}" << std::endl;

    // Iterate over the variable declarations of the environment and print.
    std::list< Behaviors::Variable * >::iterator vit;
    for( vit = _contract->environment_variables.begin();
            vit != _contract->environment_variables.end();
            ++vit )
    {
        Behaviors::Variable * current = *vit;
        std::string type = _printType(current);
        std::string name = current->GetName();

        _file << "env_vars['" << name << "'] = " 
            << type << std::endl;
    }


    _file << std::endl << std::endl <<
        "# Declarations: System Variables" << std::endl << std::endl;   
    _file << "sys_vars = {}" << std::endl;
    // Iterate over the variable declarations of the system and print.
    for( vit = _contract->system_variables.begin();
            vit != _contract->system_variables.end();
            ++vit )
    {
        Behaviors::Variable * current = *vit;
        std::string type = _printType(current);
        std::string name = current->GetName();

        _file << "sys_vars['" << name << "'] = "
            << type << std::endl;
    }
    _file << std::endl;



}

void TuLiPPrinter::_printInit()
{
    InitialConditionsFinder afinder;
    _assume->accept_visitor( afinder );

    _file << "env_init = {" << std::endl;

    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    it = afinder.initial_conditions.begin();
    while(it != afinder.initial_conditions.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != afinder.initial_conditions.end() )
        {
            _file << ",\n";
        }
    } 
    _file << "\n}" << std::endl;


    _file << std::endl;
    InitialConditionsFinder gfinder;
    _guarantee->accept_visitor( gfinder );

    _file << "sys_init = {" << std::endl;
    it = gfinder.initial_conditions.begin();
    while(it != gfinder.initial_conditions.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != gfinder.initial_conditions.end() )
        {
            _file << ",\n";
        }
    } 
    _file << "\n}" << std::endl;

    _file << std::endl << std::endl; 

}

void TuLiPPrinter::_printSafe()
{
    SafetyFinder afinder;
    _assume->accept_visitor( afinder );

    _file << "env_safe = {" << std::endl;

    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    it = afinder.safety.begin();
    while(it != afinder.safety.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != afinder.safety.end() )
        {
            _file << ",\n";
        }
        else
        {
            _file << "\n}" << std::endl;
        }
    } 

    _file << std::endl;
    SafetyFinder gfinder;
    _guarantee->accept_visitor( gfinder );

    _file << "sys_safe = {" << std::endl;
    it = gfinder.safety.begin();
    while(it != gfinder.safety.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != gfinder.safety.end() )
        {
            _file << ",\n";
        }
        else
        {
            _file << "\n}" << std::endl;
        }
    } 

    _file << std::endl << std::endl; 



}

void TuLiPPrinter::_printProgress()
{
    ProgressFinder afinder;
    _assume->accept_visitor( afinder );

    _file << "env_prog = {" << std::endl;

    std::list< Behaviors::Logics::WellFormedFormula * >::iterator it;

    it = afinder.progress.begin();
    while(it != afinder.progress.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != afinder.progress.end() )
        {
            _file << ",\n";
        }
        else
        {
            _file << "\n}" << std::endl;
        }
    } 

    _file << std::endl;
    ProgressFinder gfinder;
    _guarantee->accept_visitor( gfinder );

    _file << "sys_prog = {" << std::endl;
    it = gfinder.progress.begin();
    while(it != gfinder.progress.end())
    {
        Behaviors::Logics::WellFormedFormula * current = *it;
        SpinSyntaxWriter spinWriter;
        _file << "\t'" 
            << spinWriter.getSpinRepresentation(current)
            << "'";
        ++it;
        if( it != gfinder.progress.end() )
        {
            _file << ",\n";
        }
        else
        {
            _file << "\n}" << std::endl;
        }
    } 

    _file << std::endl << std::endl; 


}

void TuLiPPrinter::_finalize()
{
    _file << "if tulip.__version__ == \'1.3.0\' or tulip.__version__ == \'1.3.1\' or tulip.__version__ == \'1.4.0\':" 
        << std::endl;
    _file << "\tspecs = spec.GRSpec( env_vars, sys_vars, env_init, sys_init, "
        << std::endl 
            << "\t\tenv_safe, sys_safe, env_prog, sys_prog, False, False, \'\\\\A \\\\E\' )" 
        << std::endl;
    _file << "else:" << std::endl;
    _file << "\tspecs = spec.GRSpec( env_vars, sys_vars, env_init, sys_init, "
        << std::endl << "\t\tenv_safe, sys_safe, env_prog, sys_prog )"
        << std::endl << "\tomega_available=True" 
        << std::endl
        << std::endl;

    _file << "start_realizability = timer()" << std::endl; 
    _file << "if solver == 'omega':" << std::endl;
    _file << "\tstrategy = omega.is_realizable(specs)" << std::endl;
    _file << "else:" << std::endl;
    _file << "\tstrategy = gr1c.check_realizable(specs)" << std::endl;
    _file << "end_realizability = timer()" << std::endl;

    _file << "if strategy == True:" << std::endl;
    _file << "\tprint(\"Contract is CONSISTENT\")" << std::endl
        << "\trealizability_time = " << 
        "math.ceil((end_realizability - start_realizability)*10000)/10000"
        << std::endl <<
        "\tprint(str(realizability_time) + " <<
        "\"\\tseconds.\\tRealizability time.\")"
        << std::endl
        << std::endl;
    _file << "\tstart_synthesis = timer()" << std::endl;
    _file << "\tctrl = synth.synthesize(solver,specs)" << std::endl;
    _file << "\tend_synthesis = timer()" << std::endl;
    _file << "\tif args.implement == True:" << std::endl;
    _file << 
        "\t\tdumpsmach.write_python_case(implementation_output," << 
        " ctrl, classname='TulipStrategy', start='Sinit')" 
        << std::endl;
    _file << "\tend_time = timer()" << std::endl;
    _file << 
        "\tsynthesis_time = " << 
        "math.ceil((end_synthesis - start_synthesis)*10000)/10000"
        << std::endl <<
        "\tprint(str(synthesis_time) + " <<
        "\"\\tseconds.\\tSynthesis time.\")"
        << std::endl <<
        "\ttotal_time = math.ceil((end_time - start_time)*10000)/10000"
        << std::endl <<
        "\tprint( str(total_time) + \"\\tseconds.\\tTotal time.\")"
        << std::endl << std::endl;
    _file <<
        "\tif args.implement == True:" << std::endl <<
        "\t\tprint(\"A Python implementatation of the controller is " << 
        "available in \" + implementation_output)" 
        << std::endl;
    _file << "\tif args.graph == True:" << std::endl;
    _file << "\t\tctrl.save(pictorial_output)" << std::endl;
    _file << 
        "\t\tprint(\"A graphical representation of the controller " <<
        "is available in \" + pictorial_output)" 
        << std::endl << std::endl;
    _file <<
        "\tprint(\"Resulting FSM:\")" << std::endl <<
        "\tprint(str(len(ctrl.states)) + \"\tStates.\")" << std::endl <<
        "\tprint(str(len(ctrl.transitions)) + \"\tTransitions.\")" 
        << std::endl;

    //
    // Inconsistence managing.
    //
    _file <<"else:" <<std::endl;
    _file << "\tprint(\"Contract is NOT CONSISTENT\")" << std::endl 
        << std::endl;


    _file << "\tif omega_available==True and args.counterstrategy:" << std::endl <<
        "\t\tstart_counterstrategy = timer()" << std::endl <<
        "\t\tcounterstrategy = Counterstrategies.get_counterstrategy(specs)" << std::endl <<
        "\t\tstop_counterstrategy = timer();" << std::endl << 
        "\t\tCounterstrategies.print_counterexamples(" << 
        "counterstrategy, csprefix)" 
        << std::endl <<
        "\t\tcounterstrategy_time = math.ceil((stop_counterstrategy - start_counterstrategy)*10000)/10000"
        << std::endl <<
        "\telif omega_available==False:" << std::endl <<
        "\t\tprint('WARNING: Impossible to find a counterstrategy, Omega is not installed or you are using an old Tulip version.');"
    << std::endl << std::endl;


    _file << "\trealizability_time = " << 
        "math.ceil((end_realizability - start_realizability)*10000)/10000"
        << std::endl <<
        "\tprint(str(realizability_time) + " <<
        "\"\\tseconds.\\tRealizability time.\")"
        << std::endl
        << std::endl;

    _file << 
        "\tif omega_available==True and args.counterstrategy:" << std::endl <<
        "\t\tprint(str(counterstrategy_time) + '\\tseconds.\\tCounterstrategy time.')"
        << std::endl << std::endl;


    _file << "\tend_time = timer()" << std::endl;
    _file <<
        "\ttotal_time = math.ceil((end_time - start_time)*10000)/10000"
        << std::endl <<
        "\tprint(str(total_time) + \"\\tseconds.\\tTotal time.\")"
        << std::endl << std::endl;
}


std::string TuLiPPrinter::_printType( Behaviors::Variable * v )
{
    Behaviors::vartype type = v->GetVarType();
    std::stringstream ss;
    switch(type)
    {
        case t_bool:
            return std::string("'boolean'");
            break;
        case t_int:
            ss << "";
            ss << "(" << (int) v->getMinValue() 
                << "," << (int) v->getMaxValue() - 2 << ")";
            return std::string(ss.str());
            break;
        default:
            return std::string("'UNKNOWN'");
            break;
    }
}

