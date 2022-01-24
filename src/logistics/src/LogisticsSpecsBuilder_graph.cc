/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/22/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"

using namespace chase;

void LogisticsSpecsBuilder::buildGraph() const
{
    // Create a vertex for each Sink.
    for(auto b : warehouse->bays){
        auto vertex = new Vertex();
        warehouse->bays2Nodes.insert(
                std::pair< Bay*, Vertex * >(b, vertex));
        warehouse->nodes2Bays.insert(
                std::pair< Vertex*, Bay* >(vertex, b));
    }

    // Create a vertex for each Forums.
    for(auto f : warehouse->forums) {
        auto vertex = new Vertex();
        warehouse->forums2Nodes.insert(
                std::pair< Forum*, Vertex * >(f, vertex));
        warehouse->nodes2Forums.insert(
                std::pair< Vertex*, Forum* >(vertex, f));
    }

    // Create an edge for each road.
    for(auto r : warehouse->roads) {
        auto edge = new chase::Edge();
        warehouse->roads2Edges.insert(
                std::pair< Road *, Edge * >(r, edge));
        warehouse->edges2Roads.insert(
                std::pair< Edge *, Road * >(edge, r));
    }

    // Create a vertex for each Crossroad.
    for(auto c : warehouse->crossroads) {
        auto vertex = new Vertex();
        warehouse->crossroads2Nodes.insert(
                std::pair< Crossroad *, Vertex * >(c, vertex));
        warehouse->nodes2Crossroads.insert(
                std::pair< Vertex *, Crossroad * >(vertex, c));
        // Connect crossroads.
        _connectCrossroad(c);
    }



    /// \todo For each node, finds its entry and exit road.
    /// Basically, analyze its entry and exit star.
    /// Build an edge for each road. The edge is weigthed on the capacity of
    /// the road.
}


void LogisticsSpecsBuilder::_connectCrossroad(Crossroad * cross) const {
    unsigned long j = cross->position.x;
    unsigned long i = cross->position.y;

    // Analyze (if it exists) the position above.
    if(i > 0) {
        auto r = reinterpret_cast<Road*>(_components[i-1][j]);
        switch(asciimap[i-1][j]) {
            case 'U':
                cross->exits.push_back(r);
                break;
            case 'd':
                cross->entrances.push_back(r);
                break;
            default:
                break;
        }
    }
    // Analyze (if it exists) the position below.
    if(i < map_lines - 1) {
        auto r = reinterpret_cast<Road*>(_components[i+1][j]);
        switch(asciimap[i+1][j]) {
            case 'D':
                cross->exits.push_back(r);
                break;
            case 'u':
                cross->entrances.push_back(r);
                break;
            default:
                break;
        }
    }

    // Analyze (if it exists) the position on the left.
    if(j > 0) {
        auto r = reinterpret_cast<Road*>(_components[i][j-1]);
        switch (asciimap[i][j-1]) {
            case 'r':
                cross->entrances.push_back(r);
                break;
            case 'L':
                cross->exits.push_back(r);
                break;
            default:
                break;
        }
    }

    // Analyze (if it exists) the position on the left.
    if(j < map_columns) {
        auto r = reinterpret_cast<Road*>(_components[i][j-1]);
        switch (asciimap[i][j+1]) {
            case 'l':
                cross->entrances.push_back(r);
                break;
            case 'R':
                cross->exits.push_back(r);
                break;
            default:
                break;
        }
    }
}

