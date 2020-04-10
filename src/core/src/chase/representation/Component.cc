/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include <string>
#include "representation/Component.hh"


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

int Component::accept_visitor(chase::BaseVisitor &v) {
    return v.visitComponent(*this);
}

std::string Component::getString() {
    std::string ret = "Component: " + _name->getString() + " is " +
                      _definition->getName()->getString() + "\n";

    for (auto i = _params.begin(); i != _params.end(); ++i) {
        ret += "view : " + i->first + "\n";
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            std::string param = j->first;
            std::string val = j->second->getString();

            ret += ("parameter " + param + " = " + val + "\n");
        }
    }
    return ret;
}

Component *Component::clone() {
    auto ret = new Component(_definition, _name->getString());

    for(auto i = _params.begin(); i != _params.end(); ++i)
    {
        std::string view = i->first;
        std::pair< std::string,std::map< std::string, chase::Value * > > p;

        for( auto j = i->second.begin(); j != i->second.end(); ++j )
        {
            std::string param = j->first;
            Value * val = j->second->clone();
            std::pair< std::string, chase::Value * > par( param,val );

            p.second.insert(par);
        }
        p.first = view;
        ret->_params.insert(p);
    }

    return ret;
}
