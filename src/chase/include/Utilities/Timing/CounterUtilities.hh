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
 * @file	chase/include/Utilities/Timing/CounterUtilities.hh
 * @brief
 */



#ifndef COUNTER_UTILITIES_HH
#define COUNTER_UTILITIES_HH

#include <cstdlib>
#include <algorithm>

namespace Utilities
{
    namespace Timing
    {
        unsigned int getGreaterCommonFactor( unsigned int values[],
               int num_values );

        

    }
}

#endif // COUNTER_UTILITIES_HH
