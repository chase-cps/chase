/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Behaviors/Logics/UnaryLogicCombination.hh
 * @brief
 */



#ifndef UNARY_LOGIC_COMBINATION
#define UNARY_LOGIC_COMBINATION

#include "Behaviors/Logics/BooleanFormula.hh"
#include "Behaviors/Logics/UnaryFormula.hh"

namespace Behaviors 
{
    namespace Logics
    {
        class UnaryLogicCombination : 
            public Behaviors::Logics::BooleanFormula, 
            public Behaviors::Logics::UnaryFormula
        {
            public:

                UnaryLogicCombination( 
                        logic_operators op,
                        WellFormedFormula * operand );

                ~UnaryLogicCombination();

                std::string print();

                formula_type getType();

                int accept_visitor( Manipulation::BaseVisitor & v );

                chase::BaseObject * copy();

            protected:

                UnaryLogicCombination( const UnaryLogicCombination & p );
                UnaryLogicCombination & operator=( 
                        const UnaryLogicCombination & p );


        };
    }
}

#endif // UNARY_LOGIC_COMBINATION
