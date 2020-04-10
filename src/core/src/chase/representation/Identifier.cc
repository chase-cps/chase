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

Identifier::Identifier( DataDeclaration * d ) :
    Value(),
    _declaration(d)
{
    _node_type = identifier_node;
    // ASSUMPTION: The declaration must already have a parent.
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
    return _declaration->getName()->getString();
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

