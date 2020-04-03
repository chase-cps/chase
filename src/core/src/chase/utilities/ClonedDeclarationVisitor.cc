/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/ClonedDeclarationVisitor.hh"

using namespace chase;
using namespace std;

using sptr_datadecl = std::shared_ptr<DataDeclaration>;
using sptr_decl = std::shared_ptr<Declaration>;

ClonedDeclarationVisitor::ClonedDeclarationVisitor(
        std::map< sptr_decl, sptr_decl > &m ) :
        GuideVisitor(1)
{
    _map = &m;
}

int ClonedDeclarationVisitor::visitIdentifier(Identifier &o) {
    auto it = _map->find(o.getDeclaration());
    auto dec = dynamic_pointer_cast< DataDeclaration >(it->second);
    if( dec != nullptr )
        o.setDeclaration(dec);
    return 1;
}


