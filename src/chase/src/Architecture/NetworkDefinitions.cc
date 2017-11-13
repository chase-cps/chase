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
 * @file	chase/src/Architecture/NetworkDefinitions.cc
 * @brief
 */



#include "Architecture/NetworkDefinitions.hh"

using namespace Architecture;

degree_range::degree_range( unsigned int m, unsigned int M ) :
    min(m),
    max(M)
{
}


degree_range::degree_range() :
    min(0),
    max(UINT_MAX)
{
}

degree_range::~degree_range()
{
}


