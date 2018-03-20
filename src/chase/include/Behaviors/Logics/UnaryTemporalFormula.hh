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
 * @file	chase/include/Behaviors/Logics/UnaryTemporalFormula.hh
 * @brief
 */



#ifndef UNARY_TEMPORAL_FORMULA_HH
#define UNARY_TEMPORAL_FORMULA_HH

#include "TemporalFormula.hh"
#include "UnaryFormula.hh"
#include "LogicConstant.hh"

namespace Behaviors
{
    namespace Logics
    {
        class UnaryTemporalFormula : 
            public TemporalFormula,
            public UnaryFormula
        {
            public:
                UnaryTemporalFormula(
                        temporal_operators op,
                        WellFormedFormula * op1 );
                ~UnaryTemporalFormula();

                std::string print();
                formula_type getType();

                int accept_visitor( Manipulation::BaseVisitor & v );

                chase::BaseObject * copy();

            protected:

                UnaryTemporalFormula & operator=(
                        const UnaryTemporalFormula & c);
                
                UnaryTemporalFormula( const UnaryTemporalFormula & c );
        };
    }
}


#endif // UNARY_TEMPORAL_FORMULA_HH

