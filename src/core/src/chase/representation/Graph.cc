/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Graph.hh"

using namespace chase;

Graph::Graph( unsigned int size, bool directed, Name * name ) :
    _vertexes(size, nullptr), // Initialize the vertexes vector.
    _size(size),
    _directed(directed),
    _name(name)
{
    _node_type = graph_node;

    // Initialize the adjacency matrix.
    for (unsigned int i = 0; i < _size; ++i)
        for (unsigned int j = 0; j < _size; ++j) {
            AdjMatrixElement e(
                    std::pair<unsigned int, unsigned int>(i,j), nullptr);
        }
}

int Graph::accept_visitor(chase::BaseVisitor &v)
{
    return v.visitGraph(*this);
}

std::string Graph::getString() {
    std::string ret("GRAPH::\t");
    ret += _name->getString();
    ret += "\nNodes:\n";
    // Print nodes.
    for(size_t i = 0; i < _size; ++i)
    {
        ret += "\t";
        ret += std::to_string(i);
        ret += ":\t";
        if(_vertexes[i] != nullptr)
            ret += _vertexes[i]->getString();
        ret += "\n";
    }

    // Print edges.
    ret += "Edges:\n";
    std::set< Edge * >::iterator it;
    for( it = _edges.begin(); it != _edges.end(); ++it )
    {
        Edge * e = (*it);
        ret += "\t";
        ret += e->getString();
        ret += "\n";
    }

    return ret;
}

void Graph::associateVertex(unsigned int index, Vertex *vertex) {
    if(index < _size) {
        _vertexes[index] = vertex;
        vertex->setParent(this);
    }
    else messageError("Error creating the graph. Index out of size.");

}

void Graph::addEdge(Edge *edge) {
    _edges.insert(edge);
    edge->setParent(this);

    // Update the adjacency matrix.
    unsigned int s = edge->getSource();
    unsigned int t = edge->getTarget();
    AdjMatrix::iterator it;

    // Find the entry in the matrix.
    it = _matrix.find(std::pair<unsigned int, unsigned int>(s,t));
    // Set the edge as the edge in the matrix.
    if(it != _matrix.end()) it->second = edge;

    if( ! _directed ) {
        it = _matrix.find(std::pair<unsigned int, unsigned int>(t, s));
        if(it != _matrix.end()) it->second = edge;
    }
}

bool Graph::isDirected() const {
    return _directed;
}

Edge * Graph::getEdge(unsigned int source, unsigned int target) {

    for(auto it = _edges.begin(); it != _edges.end(); ++it )
    {
        Edge * edge = *it;
        if(edge->getSource() == source && edge->getTarget() == target)
            return edge;
    }
    return nullptr;
}

Vertex *Graph::getVertex(unsigned int vertex_id) {
    if( vertex_id >= _size ) return nullptr;
    return _vertexes[vertex_id];
}

Name *Graph::getName() const {
    return _name;
}

void Graph::setName(Name *name) {
    _name = name;
}

std::string Graph::getGraphViz() {
    std::string ret;
    if( _directed )
        ret = "digraph ";
    else
        ret = "graph ";
    ret += _name->getString() + "{\n";

    std::set< Edge * >::iterator it;
    for(it = _edges.begin(); it != _edges.end(); ++it)
    {
        Edge * e = *it;
        unsigned int s = e->getSource();
        unsigned int t = e->getTarget();
        ret += "\t" + std::to_string(s);
        if( _directed ) ret += "->";
        else ret += "--";
        ret += std::to_string(t) + ";\n";
    }
    ret += "}";

    return ret;
}

std::set<unsigned int> Graph::getAdjacentNodes(unsigned int id) {
    std::set< unsigned int> adjList;

    for(auto it = _edges.begin(); it != _edges.end(); ++it)
    {
        Edge * edge = *it;
        if(edge->getSource() == id)
            adjList.insert(edge->getTarget());
        if(! _directed && edge->getTarget() == id )
            adjList.insert(edge->getSource());
    }

    return adjList;
}

int Graph::getVertexIndex(std::string name)
{

    for (size_t i = 0; i < _vertexes.size(); ++i)
    {
        if( name == _vertexes[i]->getName()->getString() )
            return i;
    }
    return -1;
}

unsigned int Graph::getSize() const {
    return _size;
}

Graph::~Graph() = default;


Graph *Graph::clone() {
    /// \todo Manage the graph copy with correspondences.
    auto ret = new Graph(_size, _name->clone());

    // Clone all the vertexes.
    for( size_t n = 0; n < _vertexes.size(); ++n )
    {
        auto v = _vertexes[n]->clone();
        ret->associateVertex(n, v);
    }

    for(auto v = _edges.begin(); v != _edges.end(); ++v )
    {
        auto edge = *v;
        ret->addEdge(edge->clone());
    }

    return ret;
}
