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
 * @file	chase/include/Specification/GraphBasedFunctions.hh
 * @brief
 */



#ifndef GRAPH_BASED_SUPPORT_FUNCTIONS_HH
#define GRAPH_BASED_SUPPORT_FUNCTIONS_HH

/// @brief This file contains a set of support functions that allows to
// create logic formulae based on a graph representation of the topology.

#include "Utilities/Graphs/Graphs.hh"
#include "Specification/Problem.hh"
#include "Architecture/Network.hh"

namespace Specification
{

    /// @brief Given a path, it creates the disjunction of the state variable
    /// negations for the nodes along the path. It considers only non-constant
    /// state variables.
    Behaviors::Logics::WellFormedFormula * pathDoesNotExist(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents = true  );

    /// @brief Given a path, it creates the conjunction of the state variables
    /// for the nodes along the paths. It considers only non-constant state
    /// variables.
    Behaviors::Logics::WellFormedFormula * pathDoesExist(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents = true );

    /// @brief Given a path, it creates the conjunction of the intention 
    /// variables for the nodes along the paths. It considers only non-constant
    /// intention variables.
    Behaviors::Logics::WellFormedFormula * activatePath(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents = true );

    /// @brief Given a path, it creates the disjunction of the intention 
    /// variable negations for the nodes along the paths. It considers only
    /// non-constant intention variables.
    Behaviors::Logics::WellFormedFormula * deactivatePath(
            std::vector< int > path,
            Problem * problem,
            bool considerComponents = true );

    /// @brief Create a network from a subset of nodes in a network.
    Architecture::Network * getSubNetwork(
            Architecture::Network * network,
            std::set< Architecture::NetworkComponent * > nodes );

}


#endif // GRAPH_BASED_SUPPORT_FUNCTIONS_HH

