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
 * @file	chase/include/Behaviors/Timing/TimingDefinitions.hh
 * @brief
 */



#ifndef TIMING_DEFINITIONS_HH
#define TIMING_DEFINITIONS_HH

#include <iostream>

namespace Behaviors
{
    namespace Timing
    {
        /// @brief Enumeration of supported time units. The positions of the enum
        // are used as exponent E in 10^-E when representing time (if used units are
        // not homogeneous and they need to be uniformed, this can be useful).
        enum chase_time_unit{
            CHASE_SEC,
            CHASE_MS,
            CHASE_US,
            CHASE_NS,
            CHASE_PS,
            CHASE_FS
        };

        struct chase_time
        {
            unsigned int amount;
            chase_time_unit unit;

            chase_time(){}
            chase_time( unsigned int a, chase_time_unit u ) :
                amount(a),
                unit(u)
            {
            }

            chase_time( 
                    unsigned int a,
                    std::string u ) :
                amount(a),
                unit(CHASE_SEC)
            {
                if( u == "SEC" ) u = CHASE_SEC;
                else if( u == "MS" ) u = CHASE_MS;
                else if( u == "US" ) u = CHASE_US;
                else if( u == "NS" ) u = CHASE_NS;
                else if( u == "PS" ) u = CHASE_PS;
                else if( u == "FS" ) u = CHASE_FS;
                else
                {
                    std::cerr << "WARNING: unsupported time unit: "
                        << u << std::endl
                        << "\tUsed Seconds as a default." << std::endl;
                }
            }


            ~chase_time(){}

        };

    }
}

#endif // TIMING_DEFINITIONS_HH
