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
 * @file	chase/include/Utilities/Portability/Portability.hh
 * @brief
 */



#ifndef PORTABILITY_HH
#define PORTABILITY_HH

#include <fstream>
#include <iostream>

namespace Utilities
{
    namespace Portability
    {
        std::istream& safeGetline(std::istream& is, std::string& t);
    }
}

#endif
