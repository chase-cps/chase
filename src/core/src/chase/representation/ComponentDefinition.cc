/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/representation/ComponentDefinition.hh"
#include "representation/Contract.hh"

using namespace chase;
using namespace std;

using sptr_compdef = std::shared_ptr<ComponentDefinition>;
using sptr_name = std::shared_ptr<Name>;

ComponentDefinition::ComponentDefinition() :
    Declaration()
{
    _node_type = componentDefinition_node;
}

ComponentDefinition::~ComponentDefinition()
{
}

ComponentDefinition::ComponentDefinition(sptr_name name) :
    Declaration(name)
{
    _node_type = componentDefinition_node;
}

ComponentDefinition::ComponentDefinition(std::string name) :
    Declaration(name)
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

sptr_compdef ComponentDefinition::clone()
{
    auto ret = make_shared<ComponentDefinition>(_name);
    for( auto i = views.begin(); i != views.end(); ++i )
    {
        std::string view_name = i->first;
        Contract * view = i->second;

        std::pair< std::string, Contract * > p(view_name, 
            view->clone().get());
        ret->views.insert(p);
    }
    return ret;
}
