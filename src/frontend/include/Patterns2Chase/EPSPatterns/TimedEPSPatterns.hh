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
 * @file	frontend/include/Patterns2Chase/EPSPatterns/TimedEPSPatterns.hh
 * @brief
 */



#ifndef TIMED_EPS_PATTERNS
#define TIMED_EPS_PATTERNS

#include "Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.hh"
#include "EPS/EPSTopology.hh"

#include "Behaviors/Logics/Logics.hh"

#include "Utilities/Graphs/Graphs.hh"
#include "Behaviors/Timing/Timing.hh"

#include <iostream>
#include <vector>
#include <regex>

namespace Specification
{
    namespace EPSPatterns
    {
        class TimedEPSPattern : public HighLevelEPSPattern
        { 
            public:
                TimedEPSPattern( std::regex _regex );

                void createTimedConstraints(
                        Contracts::AGContract * contract,
                        std::vector< std::string > string );

            protected:

                virtual void _createTimedConstraints(
                        Contracts::AGContract * contract,
                        std::vector< std::string > strings ) = 0;

                Behaviors::Timing::chase_time 
                    _getTimeFromString( std::string s );

                Behaviors::Logics::WellFormedFormula * _commandDisconnection(
                        int node,
                        chase::epstool::EPS::EPSTopology * topology );

                Behaviors::Logics::WellFormedFormula * _commandPower(
                        std::vector< int > * nodes,
                        chase::epstool::EPS::EPSTopology * topology,
                        bool withNodes );

                Behaviors::Logics::WellFormedFormula * _nodeDisconnected(
                       int node, chase::epstool::EPS::EPSTopology * topology ); 

        };

        /// * must be disconnected when unhealthy within X u
        /// It should use a "Safety Counter"
        class TimedUnhealthySources : public TimedEPSPattern
        {
            public:
                TimedUnhealthySources( std::regex _regex );

            protected:

                void _createTimedConstraints(
                        Contracts::AGContract * contract,
                        std::vector< std::string > strings );

                std::vector<std::string> _extractFormulae(
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

                std::vector< Behaviors::Logics::WellFormedFormula * > 
                    _createPathFormulae(  
                            Contracts::AGContract * contract,
                            std::vector< std::string > strings );


        };

        /// * must never be unpowered from generators for more than  X u
        /// It should use a "Safety Counter"
        class TimedKeepPowered : public TimedEPSPattern
        {
            public:
                TimedKeepPowered( std::regex _regex );

            protected:

                void _createTimedConstraints(
                        Contracts::AGContract * contract,
                        std::vector< std::string > strings );

                std::vector<std::string> _extractFormulae(
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

                std::vector< Behaviors::Logics::WellFormedFormula * > 
                    _createPathFormulae(  
                            Contracts::AGContract * contract,
                            std::vector< std::string > strings );


        };

        /// Contactors are closing/opening in X u
        /// It should use a couple of "Liveness Counters"
        class TimedContactors : public TimedEPSPattern
        {
            public:
                TimedContactors( std::regex _regex );

            protected:

                void _createTimedConstraints(
                        Contracts::AGContract * contract,
                        std::vector< std::string > strings );

                std::vector<std::string> _extractFormulae(
                        std::string text );

                void _createConstraints(
                        Contracts::AGContract * contract,
                        std::vector<
                        Behaviors::Logics::WellFormedFormula * > formulae );

                std::vector< Behaviors::Logics::WellFormedFormula * > 
                    _createPathFormulae(  
                            Contracts::AGContract * contract,
                            std::vector< std::string > strings );


        };

    }
}

#endif // TIMED_EPS_PATTERNS
