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
 * @file	frontend/include/Patterns2Chase/EPSPatterns/EPSPatterns.hh
 * @brief
 */



#ifndef EPS_PATTERNS_HH
#define EPS_PATTERNS_HH

#include "Specification/TextBasedPattern.hh"
#include "Patterns2Chase/TextualParsing/TextualParsing.hh"

#include <iostream>

namespace Specification
{
    namespace EPSPatterns
    {

        // W start with {true|false}
        class Pattern_1 : public Specification::TextBasedPattern
        {
            public:
                Pattern_1( std::regex _regex ) :
                    TextBasedPattern( _regex ){}

            private:
                std::vector< std::string > _extractFormulae(
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );


        };


        // Always W
        // G(X(W))
        class Pattern_2 : public Specification::TextBasedPattern
        {
            public:
                Pattern_2( std::regex _regex ) :
                    TextBasedPattern( _regex ){}

            private:
                std::vector< std::string > _extractFormulae( 
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

        };


        // if system sensed W then do V
        // G( W -> X(V))
        class Pattern_3 : public Specification::TextBasedPattern
        {
            public:
                Pattern_3( std::regex _regex ) :
                    TextBasedPattern( _regex ){}

            private:
                std::vector< std::string > _extractFormulae( 
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

        };


        // if system is sensing W then do V
        // G( X(W) -> X(V))
        class Pattern_4 : public Specification::TextBasedPattern
        {

            public:
                Pattern_4( std::regex _regex ) :
                    TextBasedPattern( _regex ){}
            private:
                std::vector< std::string > _extractFormulae( 
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

        };


        // if system activated W and sensed Y then do Z
        // G( (W & Y) -> X(Z))
        class Pattern_5 : public Specification::TextBasedPattern
        {

            public:
                Pattern_5( std::regex _regex ) :
                    TextBasedPattern( _regex ){}

            private:
                std::vector< std::string > _extractFormulae( 
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

        };

        // do W if and only if system is sensing Y
        // G( X( (W -> Y) & (Y -> W)))
        class Pattern_6 : public Specification::TextBasedPattern
        {

            public:
                Pattern_6( std::regex _regex ) :
                    TextBasedPattern( _regex ){}

            private:
                std::vector< std::string > _extractFormulae( 
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

        };

    }
}


#endif // EPS_PATTERNS_HH
