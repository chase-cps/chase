/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
 * @brief
 */

#include "utilities/Copy.hh"

using namespace chase;

ChaseObject * copy(ChaseObject * obj )
{
    /// @todo implement the Copy Visitor.
    if( obj != nullptr ) return obj;
    return nullptr;
}

