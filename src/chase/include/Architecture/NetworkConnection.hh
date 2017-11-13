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
 * @file	chase/include/Architecture/NetworkConnection.hh
 * @brief
 */



#ifndef ARCHITECTURE_NETWORK_CONNECTION_HH
#define ARCHITECTURE_NETWORK_CONNECTION_HH

#include "Architecture/NetworkDefinitions.hh"
#include "Architecture/NetworkComponent.hh"
#include <cassert>

namespace Architecture
{
    class NetworkConnection
    {
        protected:
            NetworkComponent * _source;
            NetworkComponent * _target;
            NetworkComponent * _switch;

            bool _bidirectional;

            NetworkConnection(const NetworkConnection & );
            NetworkConnection & operator=(const NetworkConnection & );

        public:
            NetworkConnection(
                NetworkComponent * source,
                NetworkComponent * target,
                NetworkComponent * connector=NULL
            );

            ~NetworkConnection();

            bool setSource( NetworkComponent * source );
            bool setTarget( NetworkComponent * target );
            bool setSwitch( NetworkComponent * connector );
            void setBidirectional( bool b );
            bool getBidirectional();

            NetworkComponent * getSource();
            NetworkComponent * getTarget();
            NetworkComponent * getSwitch();

    };
}

#endif // ARCHITECTURE_NETWORK_CONNECTION_HH
