/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/GraphUtilities.hh"
#include "utilities/Factory.hh"
#include <algorithm>

using namespace chase;
using namespace std;

using sptr_graph = std::shared_ptr<Graph>;
using sptr_vert = std::shared_ptr<Vertex>;

void chase::findAllPathsBetweenNodes(
        sptr_graph graph,
        std::vector< unsigned int >& visited,
        unsigned int end,
        std::list< std::vector< unsigned int > > &result )
{
    unsigned int back = visited.back();

    std::set< unsigned > adjNode = graph->getAdjacentNodes(back);
    for( auto node_it = adjNode.begin(); node_it != adjNode.end(); ++node_it )
    {
        unsigned node = *node_it;

        // Check if the node has been already visited.
        if( std::find(visited.begin(), visited.end(), node) != visited.end() )
        {
            continue;
        }

        if( node == end )
        {
            visited.push_back(*node_it);
            size_t hops = visited.size();
            std::vector< unsigned > found_path;
            for( size_t hop = 0; hop < hops; ++hop )
            {
                found_path.push_back(visited[hop]);
            }

            result.push_back(found_path);
            size_t n = visited.size() - 1;
            visited.erase(visited.begin() + n);
            break;
        }
    }
    for( auto node_it = adjNode.begin(); node_it != adjNode.end(); ++node_it )
    {
        unsigned node = *node_it;
        // Check if the node has been already visited.
        if( std::find(visited.begin(), visited.end(), node) != visited.end()
            || node == end )
        {
            continue;
        }
        visited.push_back(node);
        findAllPathsBetweenNodes(graph, visited, end, result);
        size_t n = visited.size() - 1;
        visited.erase(visited.begin() + n);
    }
}

std::shared_ptr< Graph > chase::getSubGraph(
        std::shared_ptr< Graph > graph, std::set<Vertex *> vertexes)
{
    /// \todo Implement this method. For the current demo, it is not needed.
    return graph->clone();
}