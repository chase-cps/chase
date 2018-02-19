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
 * @file	chase/include/Specification/Problem.hh
 * @brief
 */



#ifndef SPECIFICATION_PROBLEM_HH
#define SPECIFICATION_PROBLEM_HH

#include "Architecture/Network.hh"
#include "Specification/Requirement.hh"
#include "Behaviors/Variable.hh"
#include "Contracts/Contracts.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Behaviors/Behaviors.hh"
#include "Behaviors/Timing/TimingDefinitions.hh"

#include "ChaseASTObject.hh"

namespace Specification
{

    class Problem : public chase::ChaseASTObject
    {
        protected:
            Architecture::Network * _network;
            
            std::map< 
                Architecture::NetworkComponent *,
                Behaviors::Variable * > _state_variables;
            std::map<
                Architecture::NetworkComponent *,
                Behaviors::Variable * > _intention_variables;

            std::map< Behaviors::Variable *,
                Behaviors::Logics::LogicProposition * > _propositions;

            Contracts::AGContract * _contract;

            Behaviors::Timing::chase_time _resolution;


            Problem( const Problem & );
            Problem & operator=( const Problem & );

            std::string _space2underscore( std::string text );

        public:

            /// @brief List of ideal components: actuation is instantaneous.
            std::set<
                Architecture::NetworkComponent * > no_delay_components;

            /// @brief List of never failing components. They do not have
            /// neither the state nor the intention variable.
            std::set<
                Architecture::NetworkComponent * > constant_components;

            /// @brief List of non controllable components. They do not have
            // intention variable.
            std::set<
                Architecture::NetworkComponent * > noncontrollable_components;

            std::set< Architecture::NetworkComponent * >
                physical_switch_on;
            std::set< Architecture::NetworkComponent * >
               physical_switch_off; 


            std::list< Requirement * > assumptions;
            std::list< Requirement * > requirements;

            Problem();
            ~Problem();

            void setNetwork( Architecture::Network * network );
            Architecture::Network * getNetwork();


            void createVariableMaps();
            Behaviors::Variable * getIntentionVariable( 
                    Architecture::NetworkComponent * c );
            Behaviors::Variable * getStateVariable(
                    Architecture::NetworkComponent * c );

            Contracts::AGContract * finalizeContract();

            Contracts::AGContract * getContract();

            void setTimeResolution( Behaviors::Timing::chase_time t );
            Behaviors::Timing::chase_time getTimeResolution();


    };

}

#endif // SPECIFICATION_PROBLEM_HH
