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
 * @file	chase/include/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.hh
 * @brief
 */



#ifndef SPECIFICATION_BASE_FUNCTIONS_LIBRARY_HH
#define SPECIFICATION_BASE_FUNCTIONS_LIBRARY_HH

#include "Specification/FunctionsLibrary.hh"

#include "Specification/BaseFunctionsLibrary/AlwaysActiveConnection.hh"
#include "Specification/BaseFunctionsLibrary/MaxFailures.hh"
#include "Specification/BaseFunctionsLibrary/MaxFailuresPerStep.hh"
#include "Specification/BaseFunctionsLibrary/MustDisconnectFailed.hh"
#include "Specification/BaseFunctionsLibrary/NeverConnect.hh"
#include "Specification/BaseFunctionsLibrary/NeverDisconnect.hh"
#include "Specification/BaseFunctionsLibrary/NoFailures.hh"
#include "Specification/BaseFunctionsLibrary/NoControl.hh"
#include "Specification/BaseFunctionsLibrary/NoRecovery.hh"
#include "Specification/BaseFunctionsLibrary/InitialState.hh"
#include "Specification/BaseFunctionsLibrary/InitialCommand.hh"
#include "Specification/BaseFunctionsLibrary/PreferActiveConnection.hh"
#include "Specification/BaseFunctionsLibrary/KeepConnectionStable.hh"
#include "Specification/BaseFunctionsLibrary/SetTimeResolution.hh"
#include "Specification/BaseFunctionsLibrary/SwitchOnTime.hh"
#include "Specification/BaseFunctionsLibrary/SwitchOffTime.hh"


namespace Specification
{
    class BaseFunctionsLibrary : public FunctionsLibrary
    {
        protected:
            
            BaseFunctionsLibrary( const FunctionsLibrary & );
            BaseFunctionsLibrary & operator=( const FunctionsLibrary & );


        public:

            BaseFunctionsLibrary();
            ~BaseFunctionsLibrary();

    };
}


#endif // SPECIFICATION_BASE_FUNCTIONS_LIBRARY_HH

