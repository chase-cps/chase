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
 * @file	chase/include/Behaviors/Logics/BinaryTemporalFormula.hh
 * @brief
 */



#ifndef BINARY_TEMPORAL_FORMULA_HH
#define BINARY_TEMPORAL_FORMULA_HH

#include "Behaviors/Logics/TemporalFormula.hh"
#include "Behaviors/Logics/BinaryFormula.hh"
#include "Behaviors/Logics/LogicConstant.hh"

namespace Behaviors
{
    namespace Logics
    {
        class BinaryTemporalFormula : 
            public TemporalFormula,
            public BinaryFormula
        {
            public:
                BinaryTemporalFormula(
                        temporal_operators op,
                        WellFormedFormula * op1,
                        WellFormedFormula * op2 );
                ~BinaryTemporalFormula();

                std::string print();

                int accept_visitor( Manipulation::BaseVisitor & v );
                
            protected:
                BinaryTemporalFormula & operator=(
                        const BinaryTemporalFormula & c);
                
                BinaryTemporalFormula( const BinaryTemporalFormula & c );
        };
    }
}


#endif // BINARY_TEMPORAL_FORMULA_HH
