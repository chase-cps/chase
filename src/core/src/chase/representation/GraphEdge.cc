/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Graph.hh"

using namespace chase;

Edge::Edge(unsigned int source, unsigned int target ) :
        _source(source),
        _target(target)
{
    _node_type = graph_edge_node;
}

unsigned int Edge::getSource() const {
    return _source;
}

void Edge::setSource(unsigned int source) {
    _source = source;
}

unsigned int Edge::getTarget() const {
    return _target;
}

void Edge::setTarget(unsigned int target) {
    _target = target;
}

int Edge::accept_visitor(chase::BaseVisitor &v) {
    return v.visitEdge(*this);
}

std::string Edge::getString() {
    std::string ret = std::to_string(_source);
    auto * p = dynamic_cast<Graph *>(_parent);
    if( p == nullptr ) messageError("Edge not in graph.");
    if(p->isDirected()) ret += " ----> ";
    else ret += " <----> ";
    ret += std::to_string(_target);
    return ret;
}



Edge::~Edge() = default;

WeightedEdge::WeightedEdge(unsigned int source, unsigned int target,
                           Value *weight) :
        Edge(source, target),
        _weight(weight)
{
    weight->setParent(this);
}

WeightedEdge::~WeightedEdge() = default;

Value *WeightedEdge::getWeight() const
{
    return _weight;
}

void WeightedEdge::setWeight(Value * weight)
{
    _weight = weight;
    weight->setParent(this);
}

std::string WeightedEdge::getString() {
    std::string ret = std::to_string(_source);
    auto p = dynamic_cast<Graph *>(_parent);
    if( p == nullptr ) messageError("Edge not in graph.");
    if(p->isDirected()) ret += " --( ";
    else ret += " <--( ";
    ret += _weight->getString();
    ret += " )--> ";
    ret += std::to_string(_target);
    return ret;
}



/// \todo Implement the clone for the edges.
Edge *Edge::clone() {
    return new Edge(_source, _target);
}

/// \todo Implement the clone for the WeightedEdges.
WeightedEdge *WeightedEdge::clone() {
    return new WeightedEdge(_source, _target, _weight->clone());
}

