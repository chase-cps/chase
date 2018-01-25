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
 * @file	chase/src/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.hh"


using namespace Specification;

BaseFunctionsLibrary::BaseFunctionsLibrary() :
    FunctionsLibrary()
{
    _functions.push_back(new SetTimeResolution());
    _functions.push_back(new AlwaysActiveConnection());
    _functions.push_back(new InitialState());
    _functions.push_back(new InitialCommand());
    _functions.push_back(new MaxFailures());
    _functions.push_back(new MaxFailuresPerStep());
    _functions.push_back(new MustDisconnectFailed());
    _functions.push_back(new NeverConnect());
    _functions.push_back(new NeverDisconnect());
    _functions.push_back(new NoFailures());
    _functions.push_back(new NoControl());
    _functions.push_back(new NoRecovery());
    _functions.push_back(new PreferActiveConnection());
    _functions.push_back(new KeepConnectionStable());
    _functions.push_back(new SwitchOnTime());
    _functions.push_back(new SwitchOffTime());

}

BaseFunctionsLibrary::~BaseFunctionsLibrary()
{
}

