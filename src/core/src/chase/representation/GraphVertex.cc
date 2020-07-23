/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "chase/representation/Graph.hh"
using namespace chase;

Vertex::Vertex(Name *name) :
    _name(name)
{
    _node_type = graph_vertex_node;
    if( _name == nullptr )
    {
        _name = new Name("GenericVertex");
    }
}

Vertex::~Vertex()
{
    delete _name;
}

Name *Vertex::getName() const {
    return _name;
}

void Vertex::setName(std::string name) {
    _name = new Name(std::move(name));
}

int Vertex::accept_visitor(chase::BaseVisitor &v) {
    return v.visitVertex(*this);
}

std::string Vertex::getString() {
    return _name->getString();
}


std::string Vertex::getGraphViz() {
    return std::string();
}

Vertex *Vertex::clone() {
    return new Vertex(_name->clone());
}
