/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/forwards.hh
 * @brief   Forward declarations for the chase library.
 */



#ifndef FORWARDS__HH
#define FORWARDS__HH

namespace chase
{
    class BaseObject;
}

namespace Behaviors
{
    class Behavior;
    class Variable;
    class Constant;
    class Function;
}

namespace Behaviors
{
    namespace Logics
    {
        class BinaryFormula;
        class BinaryLogicCombination;
        class BinaryTemporalFormula;
        class BooleanFormula;
        class LogicAtom;
        class LogicPredicate;
        class LogicConstant;
        class LogicProposition;
        class TemporalFormula;
        class UnaryFormula;
        class UnaryLogicCombination;
        class UnaryTemporalFormula;
        class WellFormedFormula;
        class GR1Formula;
        class Term;
    }

}

namespace Contracts
{
    class Contract;
    class AGContract;
}

#endif // FORWARDS__H


