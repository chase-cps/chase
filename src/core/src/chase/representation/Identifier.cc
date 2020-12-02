/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Identifier.hh"

using namespace chase;

Identifier::Identifier( DataDeclaration * d, bool primed ) :
    Value(),
    _declaration(d),
    _primed(primed)
{
    _node_type = identifier_node;
    // ASSUMPTION: The declaration must already have a parent.
    if(d->IsA() != variable_node && _primed)
        messageWarning("Primed identifier may refer only to variables.");
}

Identifier::Identifier( const Identifier &i ) :
    Value(),
    _declaration(i._declaration)
{
    _node_type = identifier_node;
}

Identifier::~Identifier()
{
}

Identifier & Identifier::operator=( const Identifier &i )
{
    _declaration = i._declaration;
    return *this;
}

DataDeclaration * Identifier::getDeclaration()
{
    return _declaration;
}

void Identifier::setDeclaration( DataDeclaration * d)
{
    _declaration = d;
}

std::string Identifier::getString()
{
    std::string ret = _declaration->getName()->getString();
    if(isPrimed()) ret += "'";
    return ret;
}

int Identifier::accept_visitor( BaseVisitor &v )
{
    return v.visitIdentifier(*this);
}

Type *Identifier::getType() {
    return getDeclaration()->getType();
}

Identifier *Identifier::clone() {
    /// \todo Fix later in the clone method of Contract the potential
    /// inconsistencies due to cloned declarations.
    return new Identifier(_declaration);
}

bool Identifier::isPrimed() const {
    return _primed;
}

void Identifier::setPrimed(bool primed) {
    _primed = primed;
}

