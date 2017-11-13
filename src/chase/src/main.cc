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
 * @file	chase/main.cc
 * @brief   Entry point of the chase tool.
 */

#include "main.hh"

using namespace chase;
using namespace chase::backend;

/// @brief Entry point of the application.
int main(int argc, char * argv[] )
{

    Params * parameters = parseCmdLine(argc, argv);

    SpecBuilder * builder = new SpecBuilder();

    chaseMessage(parameters,
            std::string("STEP1: Parsing the specification file."),
            true);

    Specification::Problem * problem = builder->parseSpecFile(
                parameters->fileIn
            );

    chaseMessage( parameters,
            std::string("STEP2: Creating data stractures to represent the library of components."),
            true);

    problem->createVariableMaps();

    std::list< std::string > _errors = builder->getErrors();


    if( _errors.empty() )
    {
        if( parameters->verbose )
        {
            std::cout << "\tParsing Successful." << std::endl;
            std::cout << "Number of instantiated Domain Specific Types: "
                << problem->getNetwork()->types.size() << std::endl;
            std::cout << "Network Architecture: " << std::endl;
            std::cout << "\tComponents:\t" << problem->getNetwork()->components.size()
                << std::endl;
            std::cout << "\tSubtypes:\t" << problem->getNetwork()->groups.size() 
                << std::endl;
            std::cout << "\tSwitches:\t" << problem->getNetwork()->switches.size()
                << std::endl;
            std::cout << "\tConnections:\t" 
                << problem->getNetwork()->connections.size()
                << std::endl;

            std::cout << std::endl;

            std::cout << "Assumptions: " << problem->assumptions.size()
                << std::endl;
            std::cout << "Requirements: " << problem->requirements.size()
                << std::endl
                << std::endl;
        }
        else
        {
            std::cout << "COMPLETED WITHOUT ERRORS" << std::endl;
        }
    }
    else
    {
        std::cerr << _errors.size() 
            << " Errors occurred during Parsing." << std::endl;
        std::list< std::string >::iterator lit;
        int i = 1;
        for(lit = _errors.begin();
                lit != _errors.end();
                ++lit )
        {
            std::cerr << "Error " << i << ":\n" << std::endl;
            std::cout << *lit << std::endl;
            std::cout << "---------------------" << std::endl;
            ++i; 
        }
    }

    chaseMessage(parameters,
            std::string("STEP 3: Build the adjacency matrix for the network."),
            true);
    problem->getNetwork()->calculateAdjacencyMatrix();

    chaseMessage(
            parameters,
            std::string("STEP 4: Load the Domain Specific library of patterns."),
            true);
    Specification::BaseFunctionsLibrary bfl;

    chaseMessage(
            parameters,
            std::string("STEP 5: Match specification with patterns."),
            true);

    chaseMessage(
            parameters,
            std::string("\tGenerate Assume-Guarantee Contract."),
            true);

    std::list< Specification::Requirement * >::iterator rit;
    for( rit = problem->assumptions.begin();
            rit != problem->assumptions.end();
            ++rit )
    {
        Specification::Requirement * curreq = *rit;
        bfl.resolveRequirement(curreq, problem);
    }

    for( rit = problem->requirements.begin();
            rit != problem->requirements.end();
            ++rit )
    {
        Specification::Requirement * curreq = *rit;
        bfl.resolveRequirement(curreq, problem);
    }



    chaseMessage(
            parameters,
            std::string("STEP 6: Simplify Contract representation."),
            true);

    Contracts::AGContract * contract = problem->finalizeContract();


    chaseMessage(
            parameters,
            std::string("\nGenerated Contract:\n"),
            true
            );

    chaseMessage(
            parameters,
            contract->print() + std::string("\n"),
            false);


    // Counters Manipulation

    chaseMessage(
            parameters,
            std::string("STEP 7: Solving timing constraints, if any."),
            true
            );
    contract->findRequiredTimePrecision();
    contract->createVariablesFromCounters();

    chaseMessage(
            parameters,
            std::string("STEP 8: Generating counters from timing constraints, if any."),
            true
            );

    manipulation::ResolveCounters * c2t = 
        new manipulation::ResolveCounters(contract);
    c2t->resolve();




    /// Backend

    chaseMessage(
            parameters,
            std::string("STEP 7: TuLiP Back-end phase."),
            true
            );

    chaseMessage(
            parameters,
            std::string("\tSimplifying logic formulae."),
            true
            );

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

    chaseMessage(
            parameters,
            std::string("\tBuiliding GR1 representation."),
            true
            );

    GR1::GR1Builder gr1_assump_builder( assumptions );
    Behaviors::Logics::GR1Formula * gr1a = gr1_assump_builder.getGR1();

    GR1::GR1Builder gr1_guarant_builder( guarantees );
    Behaviors::Logics::GR1Formula * gr1g = gr1_guarant_builder.getGR1();

    chaseMessage(
            parameters,
            std::string("\tPrinting Python file for TuLiP."),
            true
            );

    if ( parameters->backend == "tulip" )
    {
        Chase2TuLiP::TuLiPPrinter 
            printer( contract, parameters->fileOut, gr1a, gr1g );
        printer.createTuLiPFile();
    }
    else if ( parameters->backend == "gr1c" )
    {
        Chase2GR1C::GR1CPrinter 
            printer( contract, parameters->fileOut, gr1a, gr1g );
        printer.createGR1CFile();
    }

    chaseMessage(
            parameters,
            std::string("--- GENERATION COMPLETED ---\n\n"),
            true
            );



    delete builder;
}






void chase::printHelp()
{
    std::cerr << "[USAGE]\n" <<
        "chase -i input_file [-o output_file] [-V]"
        << std::endl <<
        std::endl <<
        "\t-i : specifies the txt input file containing the specifications."
        << std::endl <<
        "\t-o : specifies the txt output file. Default: input file with .py"
        "\n\t\trather than .txt extension." << std::endl <<
        "\t-b : specifies the backend to use. Default: gr1c"
        "\n\t\tLegal values: tulip, gr1c" << std::endl <<
        "\t-V : activate the verbose mode." << std::endl;
}

Params * chase::parseCmdLine( int argc, char * argv[] )
{
    Params * parameters = new Params();

    opterr = 0;
    int c;

    while(( c = getopt(argc, argv, "i:o:b:V")) != -1 )
    {
        switch(c)
        {
            case 'i':
                parameters->fileIn = std::string(optarg);
                break;
            case 'o':
                parameters->fileOut = std::string(optarg);
                break;
            case 'b':
                parameters->backend = std::string(optarg);
                if( parameters->backend != "tulip" &&
                        parameters->backend != "gr1c" )
                {
                    std::cerr << "[ERROR] Invalid backend!" << std::endl;
                    printHelp();
                    exit(-1);
                }
                break;
            case '?':
                printHelp();
                exit(-1);
            case 'V':
                parameters->verbose = true;
                break;
            default:
                printHelp();
                exit(-1);

        }
    }

    if( parameters->fileOut == "" )
    {
        size_t lastindex = parameters->fileIn.find_last_of(".");
        size_t firstindex = parameters->fileIn.find_last_of("/");
        if( firstindex > 0 ) ++firstindex;
        std::string rawname = parameters->fileIn.substr(firstindex, lastindex-firstindex);
        if( parameters->backend == "tulip" )
        {
            rawname += ".py";
        } else
        {
            rawname += ".spc";
        }
        parameters->fileOut = rawname;
    }

    std::ifstream f(parameters->fileIn.c_str());
    if( ! f.good() )
    {
        printHelp();
        f.close();
        exit(-1);
    }
    f.close();
    return parameters;
}

void chase::chaseMessage( Params * p, std::string msg, bool colored )
{
    if( p->verbose )
    {
        if( colored )
        {
            std::cout << "\033[1;32m"<< msg << "\033[0m" << std::endl;
        }
        else
        {
            std::cout << msg << std::endl;
        }
    }
}


