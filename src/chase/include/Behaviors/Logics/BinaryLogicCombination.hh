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
 * @file	chase/include/Behaviors/Logics/BinaryLogicCombination.hh
 * @brief
 */



#ifndef BINARY_LOGIC_COMBINATION_HH
#define BINARY_LOGIC_COMBINATION_HH

#include "Behaviors/Logics/BooleanFormula.hh"
#include "Behaviors/Logics/BinaryFormula.hh"

namespace Behaviors {
    namespace Logics
    {
        class BinaryLogicCombination : 
            public Behaviors::Logics::BooleanFormula,
            public Behaviors::Logics::BinaryFormula        
        {
            public:

                BinaryLogicCombination( 
                        logic_operators op,
                        WellFormedFormula * op1, 
                        WellFormedFormula * op2 );


                ~BinaryLogicCombination();

                std::string print();
                formula_type getType();

                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:

                BinaryLogicCombination( const BinaryLogicCombination & p );
                BinaryLogicCombination & operator=( 
                        const BinaryLogicCombination & p );


        };

    }
}

#endif // BINARY_LOGIC_COMBINATION
