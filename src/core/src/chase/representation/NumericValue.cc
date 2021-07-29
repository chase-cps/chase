/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/NumericValue.hh"

using namespace chase;

NumericValue::NumericValue() :
    Value()
{
    _node_type = numericValue_node;
}

NumericValue::~NumericValue() = default;


