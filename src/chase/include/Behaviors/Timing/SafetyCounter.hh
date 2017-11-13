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
 * @file	chase/include/Behaviors/Timing/SafetyCounter.hh
 * @brief
 */



#ifndef SAFETY_COUNTER_HH
#define SAFETY_COUNTER_HH

#include "Behaviors/Timing/Counter.hh"

namespace Behaviors
{
    namespace Timing
    {
        class SafetyCounter : public Counter
        {
            public:

                SafetyCounter();
                SafetyCounter( std::string name );
                ~SafetyCounter();

                void setUnsafeCondition(
                        Behaviors::Logics::WellFormedFormula * c );
                Behaviors::Logics::WellFormedFormula * getUnsafeCondition();

                void setSafetyCondition(
                        Behaviors::Logics::WellFormedFormula * c );
                Behaviors::Logics::WellFormedFormula * getSafetyCondition();

                void setCommand(
                        Behaviors::Logics::WellFormedFormula * c );
                Behaviors::Logics::WellFormedFormula * getCommand();

                void setUnsafeTolerance( chase_time t );
                chase_time getUnsafeTolerance();

                std::string print();
                        
            protected:

                Behaviors::Logics::WellFormedFormula * _command;

        };
    }
}

#endif // SAFETY_COUNTER
