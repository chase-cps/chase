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
        b->vertex = new Vertex();
        warehouse->bays2Nodes.insert(
                std::pair< Bay*, Vertex * >(b, b->vertex));
        warehouse->nodes2Bays.insert(
                std::pair< Vertex*, Bay* >(b->vertex, b));
    }


    // Create an edge for each road.
    for(auto r : warehouse->roads) {
        r->vertex = new chase::Vertex();
        warehouse->roads2Nodes.insert(
                std::pair< Road *, Vertex * >(r, r->vertex));
        warehouse->nodes2Roads.insert(
                std::pair< Vertex *, Road * >(r->vertex, r));
    }

    // Create a vertex for each Crossroad.
    for(auto c : warehouse->crossroads) {
        c->vertex = new Vertex();
        warehouse->crossroads2Nodes.insert(
                std::pair< Crossroad *, Vertex * >(c, c->vertex));
        warehouse->nodes2Crossroads.insert(
                std::pair< Vertex *, Crossroad * >(c->vertex, c));

        // Connect crossroads.
        _connectCrossroad(c);
    }

    // Create a vertex for each Forums.
    for(auto f : warehouse->forums) {
        f->vertex = new Vertex();
        warehouse->forums2Nodes.insert(
                std::pair< Forum*, Vertex * >(f, f->vertex));
        warehouse->nodes2Forums.insert(
                std::pair< Vertex*, Forum* >(f->vertex, f));

        // Connect the forum.
        _connectForum(f);
    }

    /// \todo For each node, finds its entries and exit road.
    /// Basically, analyze its entries and exit star.
    /// Build an edge for each road. The edge is weigthed on the capacity of
    /// the road.
}


void LogisticsSpecsBuilder::_connectCrossroad(Crossroad * cross) const {
    unsigned long j = cross->position.x;
    unsigned long i = cross->position.y;

    // Analyze (if it exists) the position above.
    if(i > 0) {
        auto r = _components[i-1][j];
        switch(asciimap[i-1][j]) {
            case 'U':
                cross->exits.insert(r);
                r->entries.insert(cross);
                break;
            case 'd':
            case 'D':
                cross->entries.insert(r);
                r->exits.insert(cross);
                break;
            default:
                break;
        }
    }
    // Analyze (if it exists) the position below.
    if(i < map_lines - 1) {
        auto r = _components[i+1][j];
        switch(asciimap[i+1][j]) {
            case 'D':
                cross->exits.insert(r);
                r->entries.insert(cross);
                break;
            case 'u':
            case 'U':
                cross->entries.insert(r);
                r->exits.insert(cross);
                break;
            default:
                break;
        }
    }

    // Analyze (if it exists) the position on the left.
    if(j > 0) {
        auto r = _components[i][j-1];
        switch (asciimap[i][j-1]) {
            case 'r':
            case 'R':
                cross->entries.insert(r);
                r->exits.insert(cross);
                break;
            case 'L':
                cross->exits.insert(r);
                r->entries.insert(cross);
                break;
            default:
                break;
        }
    }

    // Analyze (if it exists) the position on the Right.
    if(j < map_columns) {
        auto r = _components[i][j+1];
        switch (asciimap[i][j+1]) {
            case 'l':
            case 'L':
                cross->entries.insert(r);
                r->exits.insert(cross);
                break;
            case 'R':
                cross->exits.insert(r);
                r->entries.insert(cross);
                break;
            default:
                break;
        }
    }
}

void LogisticsSpecsBuilder::_connectForum(Forum *forum) const
{

}

void LogisticsSpecsBuilder::_connectRoad(Road *road) const
{

}