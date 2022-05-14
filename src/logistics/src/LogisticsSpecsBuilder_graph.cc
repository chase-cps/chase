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
        b->vertex = new Vertex(new Name(b->name));
        warehouse->bays2Nodes.insert(
                std::pair< Bay*, Vertex * >(b, b->vertex));
        warehouse->nodes2Bays.insert(
                std::pair< Vertex*, Bay* >(b->vertex, b));
    }


    // Create an edge for each road.
    for(auto r : warehouse->roads) {
        r->vertex = new chase::Vertex(new Name(r->name));
        warehouse->roads2Nodes.insert(
                std::pair< Road *, Vertex * >(r, r->vertex));
        warehouse->nodes2Roads.insert(
                std::pair< Vertex *, Road * >(r->vertex, r));
        _connectRoad(r);
    }

    // Create a vertex for each Crossroad.
    for(auto c : warehouse->crossroads) {
        c->vertex = new Vertex(new Name(c->name));
        warehouse->crossroads2Nodes.insert(
                std::pair< Crossroad *, Vertex * >(c, c->vertex));
        warehouse->nodes2Crossroads.insert(
                std::pair< Vertex *, Crossroad * >(c->vertex, c));

        // Connect crossroads.
        _connectCrossroad(c);
    }

    // Create a vertex for each Forums.
    for(auto f : warehouse->forums) {
        f->vertex = new Vertex(new Name(f->name));
        warehouse->forums2Nodes.insert(
                std::pair< Forum*, Vertex * >(f, f->vertex));
        warehouse->nodes2Forums.insert(
                std::pair< Vertex*, Forum* >(f->vertex, f));

        // Connect the forum.
        _connectForum(f);
    }

    // Create a vertex for each Station.
    for(auto s : warehouse->stations) {
        s->vertex = new Vertex(new Name(s->name));
        warehouse->stations2Nodes.insert(
                std::pair<PickingStation *, Vertex *>(s, s->vertex));
        warehouse->nodes2Stations.insert(
                std::pair<Vertex *, PickingStation *>(s->vertex, s));
    }

    _createGraph();

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

void LogisticsSpecsBuilder::_connectForum(Forum *forum) const {
    for(auto f : forum->coordinates) {
        auto j = f->x;
        auto i = f->y;
        if(i > 0) {
            switch (asciimap[i - 1][j]) {
                case 'r':
                case 'R':
                case 'l':
                case 'L':
                case 'P':
                    auto c = _components[i - 1][j];
                    forum->entries.insert(c);
                    forum->exits.insert(c);
                    c->entries.insert(forum);
                    c->exits.insert(forum);
                    break;
            }
        }
        if(i < (map_lines - 1)) {
            switch (asciimap[i + 1][j]) {
                case 'r':
                case 'R':
                case 'l':
                case 'L':
                case 'P':
                    auto c = _components[i + 1][j];
                    forum->entries.insert(c);
                    forum->exits.insert(c);
                    c->entries.insert(forum);
                    c->exits.insert(forum);
                    break;
            }
        }
        if(j > 0) {
            switch (asciimap[i][j - 1]) {
                case 'u':
                case 'U':
                case 'd':
                case 'D':
                case 'P':
                    auto c = _components[i][j - 1];
                    forum->entries.insert(c);
                    forum->exits.insert(c);
                    c->entries.insert(forum);
                    c->exits.insert(forum);
                    break;
            }
        }
        if(j < (map_columns - 1)) {
            switch (asciimap[i][j + 1]) {
                case 'u':
                case 'U':
                case 'd':
                case 'D':
                case 'P':
                    auto c = _components[i][j + 1];
                    forum->entries.insert(c);
                    forum->exits.insert(c);
                    c->entries.insert(forum);
                    c->exits.insert(forum);
                    break;
            }
        }
    }
}

void LogisticsSpecsBuilder::_connectRoad(Road *road) const
{
    if(road->exits.empty())
    {
        unsigned int x = road->out.x;
        unsigned int y = road->out.y;

        // if(_params->verbose) std::cout << road->name << ": " << x << " " << y << std::endl;
        switch(asciimap[y][x]){
            case 'R':
            case 'r':
                if(y < asciimap.size() - 1) if(asciimap[y+1][x] == 'D') {
                        road->exits.insert(_components[y + 1][x]);
                        _components[y+1][x]->entries.insert(road);
                    }
                if(y > 0) if(asciimap[y-1][x] == 'U') {
                        road->exits.insert(_components[y - 1][x]);
                        _components[y-1][x]->entries.insert(road);
                    }
                if(x < asciimap[y].size()-1)
                    if(asciimap[y][x+1] == 'D' || asciimap[y][x+1] == 'U') {
                        road->exits.insert(_components[y][x + 1]);
                        _components[y][x + 1]->entries.insert(road);
                    }
            break;
            case 'L':
            case 'l':
                if(y < asciimap.size() - 1) if(asciimap[y+1][x] == 'D') {
                        road->exits.insert(_components[y + 1][x]);
                        _components[y + 1][x]->entries.insert(road);
                    }
                if(y > 0) if(asciimap[y-1][x] == 'U') {
                        road->exits.insert(_components[y - 1][x]);
                        _components[y - 1][x]->entries.insert(road);
                    }
                if(x > 0)
                    if(asciimap[y][x-1] == 'D' || asciimap[y][x-1] == 'U') {
                        road->exits.insert(_components[y][x - 1]);
                        _components[y][x - 1]->entries.insert(road);
                    }
                break;
            case 'D':
            case 'd':
                if(x < asciimap[y].size() - 1) if(asciimap[y][x+1] == 'R') {
                        road->exits.insert(_components[y][x + 1]);
                        _components[y][x + 1]->entries.insert(road);
                    }
                if(x > 0) if(asciimap[y][x-1] == 'L') {
                        road->exits.insert(_components[y][x - 1]);
                        _components[y][x - 1]->entries.insert(road);
                    }
                if(y < asciimap.size() - 1)
                    if(asciimap[y+1][x] == 'L' || asciimap[y+1][x] == 'R') {
                        road->exits.insert(_components[y + 1][x]);
                        _components[y + 1][x]->entries.insert(road);
                    }
                break;
            case 'U':
            case 'u':
                if(x > asciimap[y].size() - 1) if(asciimap[y][x+1] == 'R') {
                        road->exits.insert(_components[y][x + 1]);
                        _components[y][x + 1]->entries.insert(road);
                    }
                if(x > 0) if(asciimap[y][x-1] == 'L') {
                        road->exits.insert(_components[y][x - 1]);
                        _components[y][x - 1]->entries.insert(road);
                    }
                if(y > 0)
                    if(asciimap[y-1][x] == 'L' || asciimap[y-1][x] == 'R') {
                        road->exits.insert(_components[y - 1][x]);
                        _components[y - 1][x]->entries.insert(road);
                    }
                break;
        }
    }
}

void LogisticsSpecsBuilder::_createGraph() const {
    unsigned int nodes =
            warehouse->roads.size() +
            warehouse->crossroads.size() +
            warehouse->bays.size() +
            warehouse->stations.size() +
            warehouse->forums.size();

    // Create the indexes for the nodes, and insert the nodes in the graph.

    auto graph = new Graph(nodes, true);

    unsigned int i = 0;
    for(auto c : warehouse->roads) {
        auto v = warehouse->roads2Nodes.find(c)->second;
        graph->associateVertex(i, v);
        warehouse->equipment_nodes_indexes.insert(
                std::pair<Equipment *, unsigned int>(c, i++)
        );
    }
    for(auto c : warehouse->forums) {
        auto v = warehouse->forums2Nodes.find(c)->second;
        graph->associateVertex(i, v);
        warehouse->equipment_nodes_indexes.insert(
                std::pair<Equipment *, unsigned int>(c, i++)
        );
    }
    for(auto c : warehouse->crossroads) {
        auto v = warehouse->crossroads2Nodes.find(c)->second;
        graph->associateVertex(i, v);
        warehouse->equipment_nodes_indexes.insert(
                std::pair<Equipment *, unsigned int>(c, i++)
        );
    }
    for(auto c : warehouse->bays) {
        auto v = warehouse->bays2Nodes.find(c)->second;
        graph->associateVertex(i, v);
        warehouse->equipment_nodes_indexes.insert(
                std::pair<Equipment *, unsigned int>(c, i++)
        );
    }
    for(auto c : warehouse->stations) {
        auto v = warehouse->stations2Nodes.find(c)->second;
        graph->associateVertex(i, v);
        warehouse->equipment_nodes_indexes.insert(
                std::pair<Equipment *, unsigned int>(c, i++)
        );
    }
    warehouse->graph = graph;
    _connectGraph();

    if(_params->verbose) std::cout << graph->getString() << std::endl;

}

void LogisticsSpecsBuilder::_connectGraph() const
{
    auto graph = warehouse->graph;

    for(auto it : warehouse->equipment_nodes_indexes) {
        auto equipment = it.first;
        unsigned int i = it.second;

        for(auto target : equipment->exits) {
            auto jt = warehouse->equipment_nodes_indexes.find(target);
            unsigned int j = jt->second;
            auto edge = new Edge(i, j);
            graph->addEdge(edge);
        }
    }
    if(_params->verbose)
        for(const auto & _component : _components) {
            for(auto j : _component) {
                if(j != nullptr) {
                    std::cout << j->name << "\t";
                } else std::cout << "SHELF_" << "\t";
            } std::cout << std::endl;
        }
}










