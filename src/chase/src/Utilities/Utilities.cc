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
 * @file	chase/src/Utilities/Utilities.cc
 * @brief
 */



#include "Utilities/Utilities.hh"

std::string Utilities::cleanString( std::string s )
{
    s.erase(
            std::remove(s.begin(), s.end(),' '), 
            s.end()
            );
    s.erase(
            std::remove(s.begin(), s.end(),'\n'), 
            s.end()
            );
    s.erase(
            std::remove(s.begin(), s.end(),'\t'), 
            s.end()
            );
    return s;
}
