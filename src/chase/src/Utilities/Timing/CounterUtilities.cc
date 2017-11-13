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
 * @file	chase/src/Utilities/Timing/CounterUtilities.cc
 * @brief
 */



#include "Utilities/Timing/CounterUtilities.hh"

#include <iostream>

using namespace Utilities;
using namespace Timing;

unsigned int Utilities::Timing::getGreaterCommonFactor( unsigned int values[],
       int num_values )
{
    unsigned int gcf;

    if( num_values < 1 )
    {
        return 1;
    }

    unsigned int x = values[0];
    unsigned int y = values[1];

    gcf = x;

    for( int i = 1; i < num_values; ++i )
    {
        unsigned int a = std::max(x,y);
        unsigned int b = std::min(x,y);
        unsigned int c = 1;

        do
        {
            c = a % b;
            gcf = b;

            a = b;
            b = c;
        } while( c != 0 );

         x = gcf;
         y = values[i+1];
    }

    return gcf;
}
