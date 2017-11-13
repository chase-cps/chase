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
 * @file	frontend/include/Patterns2Chase/TextualParsing/AorGVisitor.hh
 * @brief
 */



// AorGVisitor.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#ifndef A_OR_G_VISITOR_HH
#define A_OR_G_VISITOR_HH

#include "Manipulation/GuideVisitor.hh"

namespace Patterns2Chase
{
    namespace TextualParsing
    {
        /// @brief Visitor used to create Assume Guarantee contracts. It takes care
        /// to verify if a given formula is using environment or system variables.
        /// @see GuideVisitor.
        class AorGVisitor : public Manipulation::GuideVisitor
        {
            public:
                /// @brief Constructor.
                /// @param c pointer to the contract containing the declarations
                /// of the variables of the problem.
                AorGVisitor( Contracts::Contract * c );

                /// @brief Destructor.
                ~AorGVisitor();

                /// @brief visit the logic proposition and analyze where it is declared.
                /// @param o the Logic Proposition to analyze
                /// @return the default return value of the visitor (i.e., zero).
                int visitLogicProposition( Behaviors::Logics::LogicProposition & o );

                /// @brief Function returning if System variable are used or not.
                /// @return true if and only if at least one system variable has been used within the visited formula.
                bool getSysVarUsed();


            protected:
                
                Contracts::Contract * _contract;
                bool _sysVarUsed;

                AorGVisitor( const AorGVisitor & );
                AorGVisitor & operator=( const AorGVisitor & );
        };
    }
}

#endif // A_OR_G_VISITOR_HH
