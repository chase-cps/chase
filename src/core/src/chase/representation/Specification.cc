/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/Specification.hh"

using namespace chase;

Specification::Specification() {
    _node_type = specification_node;
}

Specification::~Specification() = default;
