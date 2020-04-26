/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/simplify.hh"
#include "utilities/LogicNotNormalizationVisitor.hh"

using namespace chase;

void chase::simplify(chase::ChaseObject * o)
{
    LogicNotNormalizationVisitor notNormalization;
    o->accept_visitor(notNormalization);
}


