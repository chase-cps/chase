/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "ProblemDefinitions.hh"

using namespace DSLFrontend;

Connection::Connection(Component *source, Component *target) :
    _source(source),
    _target(target)
{
    _checkConsistency();
}

std::string Connection::getSourceName() {
    return _source->name;
}

std::string Connection::getTargetName() {
    return _target->name;
}

Component *Connection::getSource() const {
    return _source;
}

Component *Connection::getTarget() const {
    return _target;
}

SwitchedConnection::SwitchedConnection(
        Component *source,
        Component *target,
        DomainSpecificType *switchType,
        Component * sw):
    Connection(source, target),
    _switchType(switchType),
    _switch(sw)
{
}

SwitchedConnection::~SwitchedConnection()
{
    delete _switch;
}

DomainSpecificType *SwitchedConnection::getSwitchType() const {
    return _switchType;
}

void SwitchedConnection::setSwitchType(DomainSpecificType *switchType) {
    _switchType = switchType;
}

Component *SwitchedConnection::getSwitch() const {
    return _switch;
}

void SwitchedConnection::setSwitch(Component *sw){
_switch = sw;
}

void Connection::_checkConsistency() {
    if( _source == nullptr )
        chase::messageError("Null source in connection.");
    if( _target == nullptr )
        chase::messageError("Null target in connection.");

    std::string source_flow, target_flow;
    // If the component is either a converter or a generic, it must consider the
    // second element of the array.
    if( _source->type->abstract_type == ct_converter ||
            _source->type->abstract_type == ct_generic )
        source_flow = _source->type->flow_parameters[1];
    else
        source_flow = _source->type->flow_parameters[0];

    // The target flow is always the first parameter in the array
    // (the input flow).
    target_flow = _target->type->flow_parameters[0];

    // Check if the flows are the same. If not, throw an error.
    if( target_flow != source_flow )
        chase::messageError(
                std::string("Incompatible flows: ") + source_flow + " -> "
                + target_flow
                );
}


SpecFunction::SpecFunction(std::string name) :
    name(std::move(name))
{
}

SpecFunction::~SpecFunction() = default;
