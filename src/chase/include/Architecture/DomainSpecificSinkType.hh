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
 * @file	chase/include/Architecture/DomainSpecificSinkType.hh
 * @brief
 */



#ifndef ARCHITECTURE_DOMAIN_SPECIFIC_SINK_TYPE
#define ARCHITECTURE_DOMAIN_SPECIFIC_SINK_TYPE

#include "Architecture/DomainSpecificType.hh"

namespace Architecture
{
    class DomainSpecificSinkType : public DomainSpecificType 
    {
        protected:

        public:
            DomainSpecificSinkType( std::string name );
            ~DomainSpecificSinkType();

            FlowParameter getReceivedFlow();
    };
}

#endif
