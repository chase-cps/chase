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
 * @file	frontend/include/DSLParser/SpecBuilder.hh
 * @brief
 */



#include "Specification/Problem.hh"
#include "ChaseLexer.h"
#include "ChaseBaseListener.h"
#include "Manipulation/findComponents.hh"


#include <iostream>


class SpecBuilder : public ChaseBaseListener 
{
    protected:
        SpecBuilder( const SpecBuilder & );
        SpecBuilder & operator=( const SpecBuilder & );

        Specification::Problem * _problem;
        Architecture::Network * _network;
        std::string _specFile;

        std::list< std::string > _errors;


        /// @brief Variables useful to manage the Switching section.
        bool _inConnections;
        bool _inSwitched;
        bool _inUnswitched;
        /// @brief A pointer to the type of component to use for instancing.
        Architecture::DomainSpecificSwitchType * _currSwitchType;

        bool _inAssumptions;
        bool _inRequirements;
        Specification::Requirement * _currRequirement;


        /// @brief Support function to retrieve the string corrensponding to
        /// a name, given a ANTLR4 ChaseParser NameContext.
        /// @param nam pointer to the NameContext to analyze.
        /// @return string corrensponding to the name stored in the NameContext.
        std::string _getNameFromContext( ChaseParser::NameContext * nam );
        std::string _getNameFromContext( Ref< ChaseParser::NameContext > name );

        Architecture::component_types _resolveType( std::string t );



        /// Support functions for connections
        void _enterSwitchedConn( ChaseParser::ConnContext * ctx );
        void _enterUnswitchedConn( ChaseParser::ConnContext * ctx );
        void _enterSpecificConn( ChaseParser::ConnContext * ctx );

        Architecture::NetworkConnection * _createConnection(
                Architecture::NetworkComponent * f,
                Architecture::NetworkComponent * t,
                bool switched );



        /// Support function to create randomly generated suffixes
        std::string _gen_random( const int len, Architecture::Network * n); 


    public:
        SpecBuilder();
        ~SpecBuilder();

        Specification::Problem * getProblem();
        Specification::Problem * parseSpecFile( std::string specFile );

        std::list< std::string > getErrors();
         


        /// Parse methods
        void enterType(ChaseParser::TypeContext * ctx);
        void enterComponent(ChaseParser::ComponentContext * ctx);
        void enterAbbrev(ChaseParser::AbbrevContext * ctx);
        
        void enterConnections(ChaseParser::ConnectionsContext * ctx);
        void exitConnections(ChaseParser::ConnectionsContext * ctx);
        void enterSwitched(ChaseParser::SwitchedContext * ctx);
        void enterUnswitched(ChaseParser::UnswitchedContext * ctx);
        void enterConn(ChaseParser::ConnContext * ctx);
        
        void enterRequirements(ChaseParser::RequirementsContext * ctx);
        void enterAssumptions(ChaseParser::AssumptionsContext * ctx);
        void exitRequirements(ChaseParser::RequirementsContext * ctx);
        void exitAssumptions(ChaseParser::AssumptionsContext * ctx);
        
        void enterReq(ChaseParser::ReqContext * ctx); 
        void enterParam(ChaseParser::ParamContext * ctx);

        virtual void visitTerminal(antlr4::tree::TerminalNode * ){}
        virtual void visitErrorNode(antlr4::tree::ErrorNode * ){}

};

