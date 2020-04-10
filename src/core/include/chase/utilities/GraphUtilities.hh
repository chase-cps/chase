/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "representation/Graph.hh"

namespace chase
{
    /// @brief Create a new graph considering only a subset of the vertexes
    /// of a graph. It preserves the edges between the vertexes.
    /// @param graph The original graph.
    /// @param vertexes The set of vertexes to preserve.
    /// @return A pointer to a new graph having only the specified vertexes.
    Graph * getSubGraph(Graph * graph, std::set< Vertex * > vertexes);

    /// @brief Function computing all the paths between a couple of nodes.
    /// The function must be call by inserting the initial node in "visited"
    /// as the first element. While "end" is the final node of the paths.
    /// @param graph The graph where to search the paths.
    /// @param visited The set of nodes already visited.
    /// @param end The ending node of the paths to be found.
    /// @param result The list of paths. I.e., the result of the procedure.
    void findAllPathsBetweenNodes(
            Graph * graph,
            std::vector< unsigned int >& visited,
            unsigned int end,
            std::list< std::vector< unsigned int > >& result
            );

}
