/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/Component.hh"

using namespace chase;

Component::Component( ComponentDefinition * definition, std::string name ) :
    _definition(definition),
    _name(new Name(name))
{
    _node_type = component_node;
}

Component::~Component() {
    delete _name;
}

ComponentDefinition * Component::getDefinition() const {
    return _definition;
}

void Component::setDefinition(ComponentDefinition * definition) {
    _definition = definition;
}

Name *Component::getName() const {
    return _name;
}

void Component::setName(Name *name) {
    _name = name;
}

void
Component::setParameter(std::string view, std::string param, Value *value)
{
    std::pair< std::string, Value * > p(param, value);
    auto it = _params.find(view);
    if( it == _params.end() )
    {
        std::pair<
                std::string,
                std::map< std::string, Value * > > v;
        v.first = view;
        v.second.insert(p);
    }
    it->second.insert(p);
}

Value *Component::getParameterValue(std::string view, std::string param)
{
    auto v = _params.find(view);
    if( v == _params.end() ) return nullptr;

    auto p = v->second.find(param);
    if( p == v->second.end() ) return nullptr;

    return p->second;
}

std::map<std::string, Value *>&
    Component::getParametersInView(std::string view)
{
    auto v = _params.find(view);

    if( v != _params.end() )
        return v->second;
    else
        return _params.end()->second;
}
