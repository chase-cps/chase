/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include <string>
#include "representation/Component.hh"


using namespace chase;
using namespace std;

using sptr_compdef = std::shared_ptr<ComponentDefinition>;
using sptr_name = std::shared_ptr<Name>;
using sptr_value = std::shared_ptr<Value>;
using sptr_comp = std::shared_ptr<Component>;

Component::Component( sptr_compdef definition, std::string name ) :
    _definition(definition),
    _name(new Name(name))
{
    _node_type = component_node;
}

Component::~Component() {
}

sptr_compdef Component::getDefinition() const {
    return _definition;
}

void Component::setDefinition(sptr_compdef definition) {
    _definition = definition;
}

sptr_name Component::getName() const {
    return _name;
}

void Component::setName(sptr_name name) {
    _name = name;
}

void
Component::setParameter(std::string view, std::string param, sptr_value value)
{
    std::pair< std::string, Value * > p(param, value.get());
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

sptr_value Component::getParameterValue(std::string view, std::string param)
{
    auto v = _params.find(view);
    if( v == _params.end() ) return nullptr;

    auto p = v->second.find(param);
    if( p == v->second.end() ) return nullptr;

    return shared_ptr<Value>(p->second);
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

sptr_comp Component::clone() {
    auto ret = make_shared<Component>(_definition, _name->getString());

    for(auto i = _params.begin(); i != _params.end(); ++i)
    {
        std::string view = i->first;
        std::pair< std::string,std::map< std::string, Value * > > p;

        for( auto j = i->second.begin(); j != i->second.end(); ++j )
        {
            std::string param = j->first;
            Value * val = dynamic_cast< Value * >(j->second->clone().get());
            std::pair< std::string, chase::Value * > par( param,val );

            p.second.insert(par);
        }
        p.first = view;
        ret->_params.insert(p);
    }

    return ret;
}
