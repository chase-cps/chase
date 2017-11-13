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
 * @file	chase/include/Behaviors/Timing/Counter.hh
 * @brief
 */



#ifndef COUNTER_HH
#define COUNTER_HH

#include "Behaviors/Logics/Logics.hh"
#include "Behaviors/Variable.hh"
#include "Behaviors/Timing/TimingDefinitions.hh"

#include <sstream>

namespace Behaviors
{
    namespace Timing
    {

        class Counter 
        {
            public:

                Counter();
                Counter( std::string name );
                ~Counter();

                void setName( std::string n );
                std::string getName();

                void setVariable( Behaviors::Variable * v );
                Behaviors::Variable * getVariable();

                std::string print();

            protected:

                std::string _name;
                Behaviors::Logics::WellFormedFormula * _trigger;
                Behaviors::Logics::WellFormedFormula * _reset;
                chase_time _time_out;
                Behaviors::Variable * _variable;

                void _setTrigger( Behaviors::Logics::WellFormedFormula * t );
                void _setReset( Behaviors::Logics::WellFormedFormula * r );
                void _setTimeOut( chase_time t );

                Behaviors::Logics::WellFormedFormula * _getTrigger();
                Behaviors::Logics::WellFormedFormula * _getReset();
                chase_time _getTimeOut();


        };
    }
}

#endif // COUNTER_HH
