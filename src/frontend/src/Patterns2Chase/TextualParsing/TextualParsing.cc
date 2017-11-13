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
 * @file	frontend/src/Patterns2Chase/TextualParsing/TextualParsing.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/TextualParsing.hh"

using namespace chase;

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace Architecture;
using namespace epstool;
using namespace EPS;
using namespace Specification;
using namespace Behaviors;
using namespace EPSPatterns;

using namespace Utilities::Portability;

FileSections::FileSections() :
    _env(),
    _sys(),
    _req()
{
}

FileSections::~FileSections()
{
}




AGContract * Parsing::parseEPSFile(
        std::string file_path,
        PatternLibrary * library,
        AGContract * contract,
        EPSTopology * topology )
{

    FileSections sections = _splitFile(file_path);


    // In the case of the EPS we need to analyze only the requirements section,
    // since the variable declarations is already done by the topology.

    std::list< std::string >::iterator it;
    for( it = sections._req.begin(); it != sections._req.end(); ++it )
    {
        std::string currentLine = *it;
        TextBasedPattern * matched = library->matchPattern(currentLine);

        if( matched == NULL )
        {
            std::cerr << "[ERROR]\n"
                << "Unmatched pattern: " << currentLine << std::endl;
        }
        else
        {
            TimedEPSPattern * epstimed = dynamic_cast< TimedEPSPattern * >(
                    matched );

            if( epstimed != NULL )
            {
                epstimed->setTopology(topology);
                std::vector< std::string > formulae = 
                    epstimed->extractFormulae(currentLine);

                epstimed->createTimedConstraints(
                        contract, formulae );
            }
            else {
                HighLevelEPSPattern * epshlp = 
                    dynamic_cast< HighLevelEPSPattern * >(
                        matched );

                if( epshlp != NULL )
                {
                    epshlp->setTopology( topology );

                    std::vector< std::string > formulae =
                        epshlp->extractFormulae(currentLine);

                    std::vector< Behaviors::Logics::WellFormedFormula * > wffs =
                        epshlp->createPathFormulae( contract, formulae );

                    epshlp->createConstraints( contract, wffs );
                }
            }




        }
    }

    return contract;


}



FileSections Parsing::_splitFile( std::string filePath )
{
    std::ifstream fd;
    fd.open(filePath.c_str(), std::ifstream::in);
    if( ! fd.good() )
    {
        std::cerr << "File " << filePath << " does not exist" << std::endl;
        exit(-1);
    }

    FileSections sections;

    bool inEnvironment = false;
    bool inSystem = false;
    bool inRequirements = false;

    std::string currentLine;

    safeGetline(fd,currentLine);
    while( ! fd.eof() )
    {
        if( currentLine == "[ENVIRONMENT]" )
        {
            inEnvironment = true;
            inSystem = false;
            inRequirements = false;
        } 
        else if ( currentLine == "[SYSTEM]" )
        {
            inEnvironment = false;
            inSystem = true;
            inRequirements = false;
        }
        else if ( currentLine == "[REQUIREMENTS]" )
        {
            inEnvironment = false;
            inSystem = false;
            inRequirements = true;
        }
        else if( currentLine != "" )
        {
            if( inEnvironment )
                sections._env.push_back(currentLine);
            else if( inSystem )
                sections._sys.push_back(currentLine);
            else if( inRequirements )
                sections._req.push_back(currentLine);
        }
        safeGetline(fd, currentLine);
    }

    return sections;

}






AGContract * Parsing::_analyzeDeclarations(
        AGContract * contract,
        std::list< Behaviors::Variable * > * varlist,
        std::list< std::string > * env )
{

    std::list< std::string >::iterator it;
    for(it = env->begin();
            it != env->end();
            ++it )
    {
        std::string currentLine = *it;
        Behaviors::Variable * var = _analyzeVariableDeclaration( currentLine );

        varlist->push_back(var);

    }

    return contract;

}




Behaviors::Variable * Parsing::_analyzeVariableDeclaration(
        std::string declaration )
{
    size_t colon_position = declaration.find(':');
    std::string name = declaration.substr(0,colon_position-1);
    std::string type = declaration.substr(colon_position+1);

    name.erase(std::remove(name.begin(),name.end(),' '),name.end());
    type.erase(std::remove(type.begin(),type.end(),' '),type.end());

    name.erase(std::remove(name.begin(),name.end(),'\t'),name.end());
    type.erase(std::remove(type.begin(),type.end(),'\t'),type.end());

    name.erase(std::remove(name.begin(),name.end(),'\n'),name.end());
    type.erase(std::remove(type.begin(),type.end(),'\n'),type.end());


    Behaviors::vartype vtype = _identifyType(type);
    if( vtype == Behaviors::vartype::t_unvalid )
    {
        std::cerr << "ERROR: unvalid data-type" << std::endl;
        exit(-1);
    }
    else
    {
        return new Variable(name, vtype, 0.0, 1.0);
    }
    return NULL;

}




Behaviors::vartype Parsing::_identifyType( std::string type )
{
    if( type == "bool" )
        return Behaviors::vartype::t_bool;
    if( type == "integer" )
        return Behaviors::vartype::t_int;
    if( type == "real" )
        return Behaviors::vartype::t_real;    
    else
        return Behaviors::vartype::t_unvalid;
}


