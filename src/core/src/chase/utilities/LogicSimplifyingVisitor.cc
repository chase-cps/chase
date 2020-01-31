/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/utilities/LogicSimplifyingVisitor.hh"

using namespace chase;

LogicSimplifyingVisitor::LogicSimplifyingVisitor( int rv ) :
    GuideVisitor(rv)
{
}


LogicSimplifyingVisitor::~LogicSimplifyingVisitor() = default;


