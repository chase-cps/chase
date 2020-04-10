/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/ClonedDeclarationVisitor.hh"

using namespace chase;

ClonedDeclarationVisitor::ClonedDeclarationVisitor(
        std::map< Declaration *, Declaration * > &m ) :
        GuideVisitor(1)
{
    _map = &m;
}

int ClonedDeclarationVisitor::visitIdentifier(Identifier &o) {
    auto it = _map->find(o.getDeclaration());
    auto dec = dynamic_cast< DataDeclaration* >(it->second);
    if( dec != nullptr )
        o.setDeclaration(dec);
    return 1;
}


