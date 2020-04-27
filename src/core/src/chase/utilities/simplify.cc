/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/simplify.hh"
#include "utilities/LogicNotNormalizationVisitor.hh"
#include "utilities/GroupTemporalOperatorsVisitor.hh"

using namespace chase;

simplify_options::simplify_options(bool _nots, bool _temporal_operators) :
        nots(_nots),
        temporal_operators(_temporal_operators)
{
}

void chase::simplify(
        chase::ChaseObject * object,
        simplify_options * options)
{
    if(options->nots) {
        LogicNotNormalizationVisitor v;
        object->accept_visitor(v);
    }
    if(options->temporal_operators){
        GroupTemporalOperatorsVisitor v;
        object->accept_visitor(v);
    }
}



