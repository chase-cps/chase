/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation.hh"
#include "GuideVisitor.hh"

namespace chase {
    class LogicSimplifyingVisitor : public GuideVisitor {
    public:
        LogicSimplifyingVisitor( int rv );
        ~LogicSimplifyingVisitor();

    protected:


    };

}