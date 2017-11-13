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
 * @file	chase/src/Manipulation/findComponents.cc
 * @brief
 */



#include "Manipulation/findComponents.hh"

namespace Manipulation
{

    std::set< Architecture::NetworkComponent * > _intersect(
            std::set< Architecture::NetworkComponent * > * s1,
            std::set< Architecture::NetworkComponent * > * s2 )
    {
        std::set< Architecture::NetworkComponent * >::iterator it1;
        std::set< Architecture::NetworkComponent * >::iterator it2;

        std::set< Architecture::NetworkComponent * > ret;

        for( it1 = s1->begin(); it1 != s1->end(); ++it1 )
        {
            Architecture::NetworkComponent * currcomp = *it1;

            it2 = s2->find(currcomp);
            if( it2 != s2->end() )
            {
                ret.insert(currcomp);
            }
        }

        return ret;

    }



    std::string _toLowerCase( std::string s )
    {
        std::transform(
                s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }


    std::list< std::string > _getTokens( std::string pattern )
    {
        /// Tokenize the initial string, in order to split the sentence into words.
        std::list< std::string > tokens;
        char * cpattern = (char *) pattern.c_str();
        char * tok;
        const char * delimiters = " ,.-";
        tok = strtok(cpattern, delimiters);
        while( tok != NULL )
        {
            tokens.push_back(std::string(tok));
            tok = strtok(NULL, delimiters);
        }
        return tokens;
    }



    /// @brief Look out if a string is contained in another.
    /// @param s1 string were to search.
    /// @param s2 string to search.
    /// @param opt Options for the search. In particular, if the search must be 
    /// exact or approximate must be specified in the options field "exact_match".
    /// @return true if s2 is matched in s1. False otherwise.
    bool _matchStrings( 
            std::string s1, 
            std::string s2, 
            const FindComponentsOptions &opt )
    {
        if( opt.exact_match )
        {
            return(s1 == s2);
        } else 
        {
            std::size_t found = s1.find(s2);
            return( found != std::string::npos );
        }
    }



    std::set< Architecture::NetworkComponent * > _getComponentsByName( 
            std::string componentName, 
            Architecture::Network * network, 
            const FindComponentsOptions & opt )
    {
        std::set< Architecture::NetworkComponent * > ret;
        std::list< Architecture::NetworkComponent * >::iterator it;
        for( it = network->components.begin();
                it != network->components.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string name = curr->getName();
            if( ! opt.case_sensitive )
            {
                name = _toLowerCase(name);
            }
            if( name==componentName )
            {
                ret.insert(curr);
            }
        }
        for( it = network->switches.begin();
                it != network->switches.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string name = curr->getName();
            if( ! opt.case_sensitive )
            {
                name = _toLowerCase(name);
            }
            if( name==componentName )
            {
                ret.insert(curr);
            }
        }

        return ret;
    }

    std::set< Architecture::NetworkComponent * > _getComponentsByAbbreviation(
            std::string componentAbbr, 
            Architecture::Network * network, 
            const FindComponentsOptions & opt )
    {
        std::set< Architecture::NetworkComponent * > ret;
        std::list< Architecture::NetworkComponent * >::iterator it;
        for( it = network->components.begin();
                it != network->components.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string name = curr->getAbbreviation();
            if( ! opt.case_sensitive )
            {
                name = _toLowerCase(name);
            }
            if( name==componentAbbr )
            {
                ret.insert(curr);
            }
        }

        for( it = network->switches.begin();
                it != network->switches.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string name = curr->getAbbreviation();
            if( ! opt.case_sensitive )
            {
                name = _toLowerCase(name);
            }
            if( name==componentAbbr )
            {
                ret.insert(curr);
            }
        }


        return ret;
    }

    std::set< Architecture::NetworkComponent * > _getComponentsByGenericType(
            std::string pattern, 
            Architecture::Network * network )
    {
        std::string types[] = {
            std::string("generic"),
            std::string("source"),
            std::string("bus"),
            std::string("converter"),
            std::string("sink"),
            std::string("switch") };

        pattern = _toLowerCase(pattern);
        Architecture::component_types t = Architecture::ct_error;
        for( unsigned short i = 0; i < Architecture::ct_error; ++i )
        {
            if( pattern == types[i] )
            {
                t = (Architecture::component_types) i;
                break;
            }
        }

        std::set< Architecture::NetworkComponent * > ret;
        if( t == Architecture::ct_error ) return ret;

        std::list< Architecture::NetworkComponent * >::iterator it;
        for( it = network->components.begin();
                it != network->components.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            Architecture::component_types ct = curr->getType()->getGenericType();
            if( t == ct )
                ret.insert( curr );
        }

        for( it = network->switches.begin();
                it != network->switches.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            Architecture::component_types ct = curr->getType()->getGenericType();
            if( t == ct )
                ret.insert( curr );
        }


        return ret;
    }


    std::set< Architecture::NetworkComponent * > _getComponentsByDomainSpecificType(
            std::string pattern, 
            Architecture::Network * network,
            const FindComponentsOptions & opt )
    {

        std::set< Architecture::NetworkComponent * > ret;
        std::list< Architecture::NetworkComponent * >::iterator it;
        for( it = network->components.begin();
                it != network->components.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string ct = curr->getType()->getName();
            if( ! opt.case_sensitive )
            {
                ct = _toLowerCase(ct);
            }
            if( pattern.find(ct) != std::string::npos )
            {
                ret.insert( curr );
            }
        }

        for( it = network->switches.begin();
                it != network->switches.end();
                ++it )
        {
            Architecture::NetworkComponent * curr = *it;
            std::string ct = curr->getType()->getName();
            if( ! opt.case_sensitive )
            {
                ct = _toLowerCase(ct);
            }
            if( pattern == ct )
                ret.insert( curr );
        }


        return ret;
    }






    std::set< Architecture::NetworkComponent * >
        _getComponentsByTopologyParameters(
                std::string pattern, 
                Architecture::Network * network,
                const FindComponentsOptions & opt )
        {    
            std::set< Architecture::NetworkComponent * > ret;

            if( ! opt.case_sensitive )
            {
                pattern = _toLowerCase(pattern);
            }

            std::list< Architecture::ComponentsGroup * >::iterator grit;
            for( grit = network->groups.begin();
                    grit != network->groups.end();
                    ++grit )
            {

                Architecture::ComponentsGroup * currgroup = *grit;
                std::list< Architecture::TopologyParameter >::iterator tpit;

                // Match tokens and parameters. Save the number of matched parameters
                // in the matched variable.
                unsigned int matched = 0;
                for( tpit = currgroup->parameters.begin();
                        tpit != currgroup->parameters.end();
                        ++tpit )
                {
                    std::string currpar = (std::string)(*tpit);
                    if( pattern.find(currpar) != std::string::npos )
                    {
                        ++matched;

                    }
                }

                if( matched )
                {
                    std::map<
                        Architecture::ComponentsGroup *,
                        std::list< Architecture::NetworkComponent * > 
                            >::iterator mit;
                    mit = network->groupsmap.find(currgroup);
                    std::list< 
                        Architecture::NetworkComponent * > * lc = &((*mit).second);

                    std::list< Architecture::NetworkComponent * >::iterator it;

                    for( it = lc->begin(); it != lc->end(); ++it )
                    {
                        Architecture::NetworkComponent * currcomp = *it;
                        ret.insert(currcomp);
                    }

                }
            }
            return ret;
        }


    ////////////////////////////////////////////////////////////////////////////////
    // FIND COMPONENTS OPTIONS
    ////////////////////////////////////////////////////////////////////////////////

    FindComponentsOptions::FindComponentsOptions() :
        verbose(false),
        case_sensitive(true),
        intersection(true),
        exact_match(false),
        name(true),
        abbreviation(true),
        generic_type(true),
        domain_specific_type(true),
        topology_parameters(true)
    {
    }

    FindComponentsOptions::~FindComponentsOptions()
    {
    }

    FindComponentsOptions::FindComponentsOptions( const FindComponentsOptions &o ) :
        verbose(o.verbose),
        case_sensitive(o.case_sensitive),
        exact_match(o.exact_match),
        name(o.name),
        generic_type(o.generic_type),
        domain_specific_type(o.domain_specific_type),
        topology_parameters(o.topology_parameters)
    {
    }

    FindComponentsOptions & FindComponentsOptions::operator=( 
            const FindComponentsOptions &o )
    {
        if(this != &o)
        {
            verbose = (o.verbose);
            case_sensitive = (o.case_sensitive);
            exact_match = (o.exact_match);
            name = (o.name);
            generic_type = (o.generic_type);
            domain_specific_type = (o.domain_specific_type);
            topology_parameters = (o.topology_parameters);
        }
        return *this;
    }



    ////////////////////////////////////////////////////////////////////////////////
    // Main find method
    ////////////////////////////////////////////////////////////////////////////////

    std::set< Architecture::NetworkComponent * > findComponents(
            std::string pattern,
            Architecture::Network * network,
            const Manipulation::FindComponentsOptions & opt )
    {

        /// Everything lower case in the case it is not case sensitive.
        if(! opt.case_sensitive )
        {
            pattern = _toLowerCase(pattern);
        }

        /// Find in the network, according to the  
        std::set< Architecture::NetworkComponent * > ret;
        std::set< Architecture::NetworkComponent * > by_name;
        std::set< Architecture::NetworkComponent * > by_abbreviation;
        std::set< Architecture::NetworkComponent * > by_generic_type;
        std::set< Architecture::NetworkComponent * > by_domain_specific_type;
        std::set< Architecture::NetworkComponent * > by_topology_parameters;

        if( opt.name )
        {
            by_name = _getComponentsByName( pattern, network, opt );
        }

        if( opt.abbreviation )
        {
            by_abbreviation = _getComponentsByAbbreviation( pattern, network, opt );
        }


        std::list< std::string > tokens;

        if( opt.generic_type )
        {
            by_generic_type = _getComponentsByGenericType( pattern, network );
        }

        if( opt.domain_specific_type )
        {
            by_domain_specific_type = 
                _getComponentsByDomainSpecificType( pattern, network, opt );
        }

        if( opt.topology_parameters )
        {
            by_topology_parameters = 
                _getComponentsByTopologyParameters( pattern, network, opt );
        }

        if( opt.intersection )
        {
            std::list< std::set< Architecture::NetworkComponent * > > nonempty;
            if( ! by_name.empty() )
            {
                nonempty.push_back(by_name);
            }
            if( ! by_abbreviation.empty() )
            {
                nonempty.push_back(by_abbreviation);
            }
            if( ! by_generic_type.empty() )
            {
                nonempty.push_back(by_generic_type);
            }
            if( ! by_domain_specific_type.empty() )
            {
                nonempty.push_back(by_domain_specific_type);
            }
            if( ! by_topology_parameters.empty() )
            {
                nonempty.push_back(by_topology_parameters);
            }
            std::list< 
                std::set< 
                Architecture::NetworkComponent * > >::iterator iter;

            iter = nonempty.begin();
            if( iter != nonempty.end())
            {
                ret = *iter;
                ++iter;
                while( iter != nonempty.end() )
                {
                    std::set< Architecture::NetworkComponent * > curr;
                    curr = *iter;
                    ret = _intersect( &ret, &curr );
                    ++iter;
                }
            }

        }
        else
        {
            /// Perform union of sets.
            ret.insert(by_name.cbegin(), by_name.cend());
            ret.insert(by_abbreviation.cbegin(), by_abbreviation.cend());
            ret.insert(by_generic_type.cbegin(), by_generic_type.cend());
            ret.insert(by_domain_specific_type.cbegin(), 
                    by_domain_specific_type.cend());
            ret.insert(by_topology_parameters.cbegin(), 
                    by_topology_parameters.cend());

        }




        return ret;
    }




}
