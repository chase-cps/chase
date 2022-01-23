/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/22/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"

using namespace chase;

void LogisticsSpecsBuilder::buildGraph() const {
    // Create a vertex for each Crossroad.
    for(auto c : warehouse->crossroads) {
        auto vertex = new Vertex();
        _crossroads2Nodes->insert(
                std::pair< Crossroad *, Vertex * >(c, vertex));
        _nodes2Crossroads->insert(
                std::pair< Vertex *, Crossroad * >(vertex, c));
    }
    // Create a vertex for each Sink.
    for(auto b : warehouse->bays){
        auto vertex = new Vertex();
        _bays2Nodes->insert(std::pair< Bay*, Vertex * >(b, vertex));
        _nodes2Bays->insert(std::pair< Vertex*, Bay* >(vertex, b));
    }

    /// \todo For each node, finds its entry and exit road.
    /// Basically, analyze its entry and exit star.
    /// Build an edge for each road. The edge is weigthed on the capacity of
    /// the road.   
}

