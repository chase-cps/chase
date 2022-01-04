/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/representation/ComponentDefinition.hh"
#include "representation/Contract.hh"

using namespace chase;

ComponentDefinition::ComponentDefinition() :
    Scope()
{
    _node_type = componentDefinition_node;
}

ComponentDefinition::~ComponentDefinition()
{
    delete _name;
}

ComponentDefinition::ComponentDefinition(Name *name) :
    Scope(name)
{
    _node_type = componentDefinition_node;
}

ComponentDefinition::ComponentDefinition(std::string name) :
    Scope(name)
{
    _node_type = componentDefinition_node;
}

int ComponentDefinition::accept_visitor(chase::BaseVisitor &v)
{
    return v.visitComponentDefinition(* this);
}

std::string ComponentDefinition::getString()
{
    std::string ret = "Contract definition: " + _name->getString();
    ret += "\nViews:\n";
    for( auto i = views.begin(); i != views.end(); ++i )
    {
        ret += i->first + "\n" + i->second->getString();
    }
    return ret;
}

ComponentDefinition * ComponentDefinition::clone()
{
    auto ret = new ComponentDefinition(_name->clone());
    for( auto i = views.begin(); i != views.end(); ++i )
    {
        std::string view_name = i->first;
        Contract * view = i->second;

        std::pair< std::string, Contract * > p(view_name, view->clone());
        ret->views.insert(p);
    }

    for( const auto & i : subcomponents )
    {
        ret->subcomponents.insert(i->clone());
    }

    return ret;
}
