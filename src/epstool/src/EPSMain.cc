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
 * @file	chase/src/Architecture/EPS/EPSMain.cc
 * @brief
 */

#include "EPSMain.hh"


using namespace chase;
using namespace backend;
using namespace chase::epstool::EPS;
using namespace Utilities::Graphs;
using namespace Contracts;
using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Chase2TuLiP;
using namespace GR1;
using namespace manipulation;
using namespace epstool;



int main( int argc, char * argv[] )
{
    Params * parameters = parseCmdLine(argc, argv);

    /// Frontend

    EPSTopology * topology = new EPSTopology( 
            parameters->fileInArchitecture,
            parameters->physical );

    AGContract * contract = topology->buildContractFromTopology();

    contract = TextualParsing::Parsing::parseEPSFile(
            parameters->fileInBehavior,
            new EPSTimedPatternLibrary(),
            contract,
            topology );


    // Counters Manipulation

    contract->findRequiredTimePrecision();
    contract->createVariablesFromCounters();

    ResolveCounters * c2t = new ResolveCounters(contract);
    c2t->resolve();


    

    /// Backend

    Behaviors::Logics::WellFormedFormula * assumptions = 
        static_cast< Behaviors::Logics::WellFormedFormula * >(
                contract->getAssumptions());

    Behaviors::Logics::WellFormedFormula * guarantees =
        static_cast< Behaviors::Logics::WellFormedFormula * >(
                contract->getGuarantees());

    // Simplify Formulas
    
    Behaviors::Logics::BinaryLogicCombination * gb = 
        dynamic_cast< Behaviors::Logics::BinaryLogicCombination * >(
                guarantees );

    Behaviors::Logics::BinaryLogicCombination * ab = 
        dynamic_cast< Behaviors::Logics::BinaryLogicCombination * >(
                assumptions );

    if( gb != NULL )
    {
        Manipulation::SimplifyVisitor v;
        v.visitBinaryLogicCombination(*gb);
    }
    if( ab != NULL )
    {
        Manipulation::SimplifyVisitor v;
        v.visitBinaryLogicCombination(*ab);
    }

    if(parameters->verbose)
    {
        std::cout << contract->print() << std::endl;
    }

    GR1Builder gr1_assump_builder( assumptions );
    Behaviors::Logics::GR1Formula * gr1a = gr1_assump_builder.getGR1();

    GR1Builder gr1_guarant_builder( guarantees );
    Behaviors::Logics::GR1Formula * gr1g = gr1_guarant_builder.getGR1();

    TuLiPPrinter printer( contract, parameters->fileOut.c_str(), gr1a, gr1g );
    printer.createTuLiPFile();

    delete(topology);

}





void chase::epstool::printHelp()
{
    std::cerr << "[USAGE]\n" <<
        "epstool -b behaviors_specs_file -a architecture_file -o output_file [-p -V]"
        << std::endl;
}


Params * chase::epstool::parseCmdLine( int argc, char * argv[] )
{
    Params * parameters = new Params();

    opterr = 0;
    int c;

    while(( c = getopt(argc, argv, "b:a:o:pV")) != -1 )
    {
        switch(c)
        {
            case 'b':
                parameters->fileInBehavior = std::string(optarg);
                break;
            case 'a':
                parameters->fileInArchitecture = std::string(optarg);
                break;
            case 'o':
                parameters->fileOut = std::string(optarg);
                break;
            case 'p':
                parameters->physical = true;
                break;
            case '?':
                printHelp();
                exit(-1);
            case 'V':
                parameters->verbose = true;
                break;
            default:
                exit(-1);

        }

    }
    return parameters;
}



