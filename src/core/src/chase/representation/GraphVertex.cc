/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "chase/representation/Graph.hh"
using namespace chase;
using namespace std;

using sptr_name = std::shared_ptr<Name>;
using sptr_vert = std::shared_ptr<Vertex>;

Vertex::Vertex(sptr_name name) :
    _name(name)
{
    _node_type = graph_vertex_node;
    if( _name == nullptr )
    {
        _name = make_shared<Name>("GenericVertex");
    }
}

Vertex::~Vertex()
{
}

sptr_name Vertex::getName() const {
    return _name;
}

void Vertex::setName(std::string name) {
    _name = make_shared<Name>(std::move(name));
}

int Vertex::accept_visitor(chase::BaseVisitor &v) {
    return v.visitVertex(*this);
}

std::string Vertex::getString() {
    return _name->getString();
}


sptr_vert Vertex::clone() {
    return make_shared<Vertex>(_name->clone());
}
