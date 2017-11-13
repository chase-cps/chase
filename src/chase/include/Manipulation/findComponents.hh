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
 * @file	chase/include/Manipulation/findComponents.hh
 * @brief
 */



#ifndef MANIPULATION_FIND_COMPONENTS_HH
#define MANIPULATION_FIND_COMPONENTS_HH

#include <set>
#include <list>
#include <algorithm>
#include "Architecture/Network.hh"
#include <string.h>

namespace Manipulation
{
    struct FindComponentsOptions
    {
        bool verbose;
        bool case_sensitive;
        /// @brief Intersection only of NON-EMPTY sets.
        bool intersection;
        bool exact_match;
        bool name;
        bool abbreviation;
        bool generic_type;
        bool domain_specific_type;
        bool topology_parameters;

        /// \todo
        // Implement the search for flow type. For instance to find all the
        // components using a given flow parameter. This means that it must be
        // possible to set which parameter is to consider (e.g., input/output).

        FindComponentsOptions();
        ~FindComponentsOptions();

        FindComponentsOptions( const FindComponentsOptions & );
        FindComponentsOptions & operator=( const FindComponentsOptions & );

    };

    std::set< Architecture::NetworkComponent * > _intersect(
            std::set< Architecture::NetworkComponent * > * s1,
            std::set< Architecture::NetworkComponent * > * s2 );

    std::set< Architecture::NetworkComponent * > findComponents(
            std::string pattern,
            Architecture::Network * network,
            const FindComponentsOptions & opt = FindComponentsOptions()
            );


    std::list< std::string > _getTokens( std::string pattern );
}


#endif // MANIPULATION_FIND_COMPONENTS_HH
