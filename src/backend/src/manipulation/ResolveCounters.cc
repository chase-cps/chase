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
 * @file	backend/src/Chase2TuLiP/manipulation/ResolveCounters.cc
 * @brief
 */



#include "manipulation/ResolveCounters.hh"

using namespace chase::backend::manipulation;

using namespace Behaviors;
using namespace Timing;
using namespace Logics;


ResolveCounters::ResolveCounters( Contracts::AGContract * contract ) :
    _contract(contract)
{
}

ResolveCounters::~ResolveCounters()
{
}

void ResolveCounters::resolve()
{
    std::list< Behaviors::Timing::SafetyCounter * >::iterator sit;
    for( sit = _contract->safety_counters.begin();
            sit != _contract->safety_counters.end();
            ++sit )
    {
        _resolveSafetyCounter(*sit);
    }

    std::list< Behaviors::Timing::LivenessCounter * >::iterator lit;
    for( lit = _contract->liveness_counters.begin();
            lit != _contract->liveness_counters.end();
            ++lit )
    {
        _resolveLivenessCounter(*lit);
    }
}

void ResolveCounters::_resolveLivenessCounter(LivenessCounter * counter)
{
    WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
            _contract->assumptions );

    if( assumptions == NULL )
    {
        std::cerr << "[ERROR] Assumptions are not expressed using logics"
            << std::endl;
        exit(-1);
    }
    // Liveness condition: [](!(C>timeout ^ ~reset))

    std::string time_condition_name = counter->getVariable()->GetName();
    chase_time tolerance = counter->getMaxTimeRequired();
    chase_time granularity = _contract->time_precision;

    int normalized_time = (tolerance.amount * 
            pow(10, tolerance.unit - granularity.unit)) / granularity.amount;

    time_condition_name += ">";
    time_condition_name += std::to_string(normalized_time);



    // Reset condition [](R -> (C=0))
    {
        std::string teq0 = counter->getVariable()->GetName();
        teq0 += "=0";

        LogicProposition * propEq0 = new LogicProposition(teq0);
        WellFormedFormula * condition = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    counter->getDesiredCondition(),
                    counter->getActivationCondition()));

        WellFormedFormula * keepSteady = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    propEq0,
                    counter->getDesiredCondition()));

        WellFormedFormula * stopCounter = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_globally,
                    new BinaryLogicCombination(
                        op_implies,
                        condition,
                        new UnaryTemporalFormula(
                            op_next,
                            keepSteady))));

        assumptions = static_cast< WellFormedFormula * >( 
                new BinaryLogicCombination(
                    op_land, assumptions, stopCounter ));

    }


    // Update phase

    int max = static_cast< int >( counter->getVariable()->getMaxValue() );

    for( int i = 0; i < max - 2; ++i )
    {
        std::string eq(counter->getVariable()->GetName());
        eq += "=";
        eq += std::to_string(i);

        LogicProposition * propEq = new LogicProposition(eq);

        std::string eqX(counter->getVariable()->GetName());
        eqX += "=";
        eqX += std::to_string(i+1);

        LogicProposition * propEqX = new LogicProposition(eqX);

        std::string eq0(counter->getVariable()->GetName());
        eq0 += "=0";

        LogicProposition * propEq0 = new LogicProposition(eq0);

        WellFormedFormula * condition = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    propEq,
                    new BinaryLogicCombination(
                        op_land,
                        counter->getActivationCondition(),
                        new UnaryLogicCombination(
                            op_lnot,
                            counter->getDesiredCondition()))));

        WellFormedFormula * notFine = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    propEqX,
                    new UnaryLogicCombination(
                        op_lnot,
                        counter->getDesiredCondition())));

        WellFormedFormula * fine = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    propEq0,
                    counter->getDesiredCondition()));

        WellFormedFormula * impl = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_implies,
                    condition,
                    new UnaryTemporalFormula(
                        op_next,
                        new BinaryLogicCombination(
                            op_lor,
                            notFine,
                            fine
                            )
                        )
                    )
                );

        WellFormedFormula * always = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_globally,
                    impl ));

        assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    assumptions,
                    always ));
    }


    /// MAX TIME MUST ACTUATE!!!

    std::string eq(counter->getVariable()->GetName());
    eq += "=";
    eq += std::to_string(max-2);

    LogicProposition * propEq = new LogicProposition(eq);

    std::string eq0(counter->getVariable()->GetName());
    eq0 += "=0";

    LogicProposition * propEq0 = new LogicProposition(eq0);

    WellFormedFormula * condition = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                propEq,
                new BinaryLogicCombination(
                    op_land,
                    counter->getActivationCondition(),
                    new UnaryLogicCombination(
                        op_lnot,
                        counter->getDesiredCondition()))));

    WellFormedFormula * fine = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                propEq0,
                counter->getDesiredCondition()));

    WellFormedFormula * impl = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_implies,
                condition,
                new UnaryTemporalFormula(
                    op_next,
                    fine
                    )
                )
            );

    WellFormedFormula * always = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                impl ));

    assumptions = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                always ));

    assumptions = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                propEq0 ));


    // Integration in the contract
    _contract->assumptions = assumptions;





}

void ResolveCounters::_resolveSafetyCounter(SafetyCounter * counter)
{
    WellFormedFormula * guarantees = dynamic_cast< WellFormedFormula * >(
            _contract->guarantees );

    if( guarantees == NULL )
    {
        std::cerr << "[ERROR] Guarantees are not expressed using logics"
            << std::endl;
        exit(-1);

    }


    // Time Condition

    std::string propLeqName( (counter->getVariable())->GetName() );

    std::stringstream ss;

    chase_time tolerance = counter->getUnsafeTolerance();
    chase_time granularity = _contract->time_precision;

    int normalized_tolerance = 
        (tolerance.amount * 
         pow(10, tolerance.unit - granularity.unit)/ granularity.amount);
    /*
    ss << normalized_tolerance;

      propLeqName +="<";
        propLeqName += ss.str();
        ss.flush();

        LogicProposition * propLeq = new LogicProposition( propLeqName );

        WellFormedFormula * tcondition = static_cast< WellFormedFormula * >(
        new UnaryTemporalFormula( op_globally, propLeq ));


        guarantees = static_cast< WellFormedFormula * >(
        new BinaryLogicCombination( op_land, guarantees, tcondition ));
        
*/
    // Update phase

    LogicProposition * propEq0;
    for( int i = 0; i < normalized_tolerance; ++i )
    {
        std::string eq(counter->getVariable()->GetName());
        eq += "=";
        eq += std::to_string(i);

        LogicProposition * propEq = new LogicProposition(eq);

        std::string eqX(counter->getVariable()->GetName());
        eqX += "=";
        eqX += std::to_string(i+1);
        ss.flush();

        LogicProposition * propEqX = new LogicProposition(eqX);

        std::string eq0(counter->getVariable()->GetName());
        eq0 += "=0";

        propEq0 = new LogicProposition(eq0);

        WellFormedFormula* condition = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    counter->getUnsafeCondition(),
                    propEq));

        WellFormedFormula * notFine = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    new UnaryLogicCombination(
                        op_lnot,
                        counter->getSafetyCondition()),
                    propEqX));

        WellFormedFormula * fine = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    propEq0,
                    counter->getSafetyCondition()));


        WellFormedFormula * imp;
        if( i < normalized_tolerance-1 )
        {
        imp = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_implies,
                        condition,
                        new UnaryTemporalFormula(
                            op_next,
                            new BinaryLogicCombination(
                                op_lor,
                                notFine,
                                fine))));
        } else {
        imp = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_implies,
                        condition,
                        new UnaryTemporalFormula(
                            op_next, fine)));
        }



        WellFormedFormula * always = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_globally,
                    imp ));

        guarantees = static_cast< WellFormedFormula * >( 
                new BinaryLogicCombination(
                    op_land,
                    guarantees,
                    always));





    }
    guarantees = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                guarantees,
                propEq0 ));

    WellFormedFormula * safety_reached = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                new BinaryLogicCombination(
                    op_implies,
                    counter->getSafetyCondition(),
                    propEq0)));

    WellFormedFormula * notunsafe = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                new BinaryLogicCombination(
                    op_implies,
                    new UnaryLogicCombination(
                        op_lnot,
                        counter->getUnsafeCondition()),
                    propEq0)));

    guarantees = static_cast< WellFormedFormula * >( 
            new BinaryLogicCombination(
                op_land,
                guarantees,
                safety_reached));

    guarantees = static_cast< WellFormedFormula * >( 
            new BinaryLogicCombination(
                op_land,
                guarantees,
                notunsafe));


    _contract->guarantees = guarantees;

} 
