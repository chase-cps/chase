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
 * @file	chase/include/Behaviors/Timing/LivenessCounter.hh
 * @brief
 */



#ifndef LIVENESS_COUNTER_HH
#define LIVENESS_COUNTER_HH

#include "Behaviors/Timing/Counter.hh"

namespace Behaviors
{
    namespace Timing
    {
        class LivenessCounter : public Counter
        {
            public:
                LivenessCounter();
                LivenessCounter( std::string name );
                ~LivenessCounter();

                void setActivationCondition(
                        Behaviors::Logics::WellFormedFormula * c );
                Behaviors::Logics::WellFormedFormula * getActivationCondition();

                void setDesiredCondition(
                        Behaviors::Logics::WellFormedFormula * c );
                Behaviors::Logics::WellFormedFormula * getDesiredCondition();

                void setMaxTimeRequired( chase_time t );
                chase_time getMaxTimeRequired();

            protected:
                
        };
    }
}


#endif // LIVENESS_COUNTER_HH
