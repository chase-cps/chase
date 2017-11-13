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
 * @file	chase/src/Specification/GraphBasedFunctions.cc
 * @brief
 */



#include "Specification/GraphBasedFunctions.hh"

using namespace Behaviors;
using namespace Behaviors::Logics;


namespace Specification
{

    WellFormedFormula * pathDoesNotExist(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents  )
    {
        WellFormedFormula * ret = static_cast< WellFormedFormula * >(
                new LogicConstant(false));

        Architecture::Network * network = problem->getNetwork();

        for( size_t i = 0; i < path.size(); ++i )
        {
            unsigned int currindex = (unsigned int) path[i];
            std::map< unsigned int, 
                Architecture::NetworkComponent * >::iterator it;
            it = network->indexesMap.find(currindex);

            if( it == network->indexesMap.end() )
            {
                std::cerr << "[WARNING] Something bad happened searching paths"
                    << std::endl;
                continue;
            }

            Architecture::NetworkComponent * currcomp = (*it).second;
            if( considerComponents == false )
            {
                if( currcomp->getType()->getGenericType() !=
                        Architecture::ct_switch)
                {
                    continue;
                }
            }

            std::set< Architecture::NetworkComponent * >::iterator findit;
            findit = problem->constant_components.find(currcomp);
            if(findit == problem->constant_components.end() )
            {
                Behaviors::Variable * var = problem->getStateVariable(currcomp); 
                ret = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_lor,
                            ret,
                            new UnaryLogicCombination(
                                op_lnot,
                                new LogicProposition(var->GetName()))));
            }
        }
        return ret;
    }

    WellFormedFormula * pathDoesExist(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents  )
    {
        WellFormedFormula * ret = static_cast< WellFormedFormula * >(
                new LogicConstant(true));

        Architecture::Network * network = problem->getNetwork();

        for( size_t i = 0; i < path.size(); ++i )
        {
            unsigned int currindex = (unsigned int) path[i];
            std::map< unsigned int, 
                Architecture::NetworkComponent * >::iterator it;
            it = network->indexesMap.find(currindex);

            if( it == network->indexesMap.end() )
            {
                std::cerr << "[WARNING] Something bad happened searching paths"
                    << std::endl;
                continue;
            }

            Architecture::NetworkComponent * currcomp = (*it).second;
            if( considerComponents == false )
            {
                if( currcomp->getType()->getGenericType() !=
                        Architecture::ct_switch)
                {
                    continue;
                }
            }

            std::set< Architecture::NetworkComponent * >::iterator findit;
            findit = problem->constant_components.find(currcomp);
            if(findit == problem->constant_components.end() )
            {
                Behaviors::Variable * var = problem->getStateVariable(currcomp); 
                ret = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            ret,
                            new LogicProposition(var->GetName())));
            }
        }
        return ret;

    }


    WellFormedFormula * activatePath(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents  )
    {
        WellFormedFormula * ret = static_cast< WellFormedFormula * >(
                new LogicConstant(true));

        Architecture::Network * network = problem->getNetwork();

        for( size_t i = 0; i < path.size(); ++i )
        {
            unsigned int currindex = (unsigned int) path[i];
            std::map< unsigned int, 
                Architecture::NetworkComponent * >::iterator it;
            it = network->indexesMap.find(currindex);

            if( it == network->indexesMap.end() )
            {
                std::cerr << "[WARNING] Something bad happened searching paths"
                    << std::endl;
                continue;
            }

            Architecture::NetworkComponent * currcomp = (*it).second;
            if( considerComponents == false )
            {
                if( currcomp->getType()->getGenericType() !=
                        Architecture::ct_switch)
                {
                    continue;
                }
            }


            std::set< Architecture::NetworkComponent * >::iterator findit;
            findit = problem->noncontrollable_components.find(currcomp);
            if(findit == problem->noncontrollable_components.end() )
            {
                Behaviors::Variable * var = 
                    problem->getIntentionVariable(currcomp); 
                ret = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            ret,
                            new LogicProposition(var->GetName())));
            }
        }

        return ret;
    }

    WellFormedFormula * deactivatePath(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents  )
    {
        WellFormedFormula * ret = static_cast< WellFormedFormula * >(
                new LogicConstant(false));

        Architecture::Network * network = problem->getNetwork();

        for( size_t i = 0; i < path.size(); ++i )
        {
            unsigned int currindex = (unsigned int) path[i];
            std::map< unsigned int, 
                Architecture::NetworkComponent * >::iterator it;
            it = network->indexesMap.find(currindex);

            if( it == network->indexesMap.end() )
            {
                std::cerr << "[WARNING] Something bad happened searching paths"
                    << std::endl;
                continue;
            }

            Architecture::NetworkComponent * currcomp = (*it).second;
            if( considerComponents == false )
            {
                if( currcomp->getType()->getGenericType() !=
                        Architecture::ct_switch)
                {
                    continue;
                }
            }

            std::set< Architecture::NetworkComponent * >::iterator findit;
            findit = problem->noncontrollable_components.find(currcomp);
            if(findit == problem->noncontrollable_components.end() )
            {
                Behaviors::Variable * var = 
                    problem->getIntentionVariable(currcomp); 
                ret = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_lor,
                            ret,
                            new UnaryLogicCombination(
                                op_lnot,
                                new LogicProposition(var->GetName()))));
            }
        }

        return ret;

    }

    Architecture::Network * getSubNetwork(
            Architecture::Network * network,
            std::set< Architecture::NetworkComponent * > nodes )
    {
        Architecture::Network * ret = new Architecture::Network();

        std::set< Architecture::NetworkComponent * > switches;
        std::set< Architecture::NetworkConnection * > connections;

        std::set< Architecture::NetworkComponent * >::iterator sit;
        for( sit = nodes.begin();
                sit != nodes.end();
                ++sit )
        {
            Architecture::NetworkComponent * currcomp = *sit;
            ret->components.push_back(currcomp);
        }

        std::list< Architecture::NetworkConnection * >::iterator connit;
        for( connit = network->connections.begin();
                connit != network->connections.end();
                ++connit )
        {
            Architecture::NetworkConnection * currconn = *connit;
            Architecture::NetworkComponent * from = currconn->getSource();
            Architecture::NetworkComponent * to = currconn->getTarget();
            Architecture::NetworkComponent * connector = currconn->getSwitch();

            /// Check if the "to" and "from" nodes of the current connection are
            /// nodes in the subset to use.
            std::set< Architecture::NetworkComponent * >::iterator finder;
            finder = nodes.find(from);
            if( finder == nodes.end() ) continue;
            finder = nodes.find(to);
            if( finder == nodes.end() ) continue;

            if( connector != NULL )
                switches.insert(connector);

            connections.insert(currconn);
        }

        std::set< Architecture::NetworkComponent * >::iterator cit;
        for( cit = switches.begin();
                cit != switches.end();
                ++cit )
        {
            ret->switches.push_back(*cit);
        }

        std::set< Architecture::NetworkConnection * >::iterator it;
        for( it = connections.begin();
                it != connections.end();
                ++it )
        {
            ret->connections.push_back(*it);
        }

        return ret;
    }


}
