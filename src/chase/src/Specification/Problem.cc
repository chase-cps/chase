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
 * @file	chase/src/Specification/Problem.cc
 * @brief
 */



#include "Specification/Problem.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Behaviors::Timing;


Problem::Problem() :
    BaseObject(),
    _network(NULL),
    _state_variables(),
    _intention_variables(),
    _contract(NULL),
    _resolution(0, CHASE_SEC),
    no_delay_components(),
    constant_components(),
    noncontrollable_components(),
    physical_switch_on(),
    physical_switch_off(),
    assumptions(),
    requirements()
{
    _contract = new Contracts::AGContract();
    _contract->setParent( this );
}

Problem::~Problem()
{
}

std::string Problem::_space2underscore( std::string text )
{
    for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
        if(*it == ' ') {
            *it = '_';
        }
    }
    return text;
}

void Problem::setNetwork( Architecture::Network * network )
{
    _network = network;
}

Architecture::Network * Problem::getNetwork()
{
    return _network;
}

void Problem::createVariableMaps()
{

    std::list< Architecture::NetworkComponent * >::iterator cit;

    for( cit = _network->components.begin();
            cit != _network->components.end();
            ++cit )
    {
        Architecture::NetworkComponent * curr = *cit;

        std::string abbreviation = curr->getAbbreviation();
        std::string name;
        if( abbreviation != "" )
        {
            name = abbreviation;
        }
        else
        {
            name = curr->getName();
        }

        name = _space2underscore( name );

        // Is the component never failing?
        std::string state = name + std::string("_state");
        std::string intention = name + std::string("_intention");

        Behaviors::Variable * statevar = 
            new Behaviors::Variable(state, Behaviors::t_bool);
        Behaviors::Variable * intentvar =
            new Behaviors::Variable(intention, Behaviors::t_bool);

        _state_variables.insert( 
                std::pair< Architecture::NetworkComponent *,
                Behaviors::Variable * >(curr, statevar));


        _intention_variables.insert( 
                std::pair< Architecture::NetworkComponent *,
                Behaviors::Variable * >(curr, intentvar));


        _contract->system_variables.push_back(intentvar);
        _contract->environment_variables.push_back(statevar);
    }

    for( cit = _network->switches.begin();
            cit != _network->switches.end();
            ++cit )
    {
        Architecture::NetworkComponent * curr = *cit;

        std::string abbreviation = curr->getAbbreviation();
        std::string name;
        if( abbreviation != "" )
        {
            name = abbreviation;
        }
        else
        {
            name = curr->getName();
        }

        name = _space2underscore( name );

        // Is the component never failing?
        if( true ) //constant_components.find(curr) 
            //== constant_components.end() )
        {
            std::string state = name + std::string("_state");
            std::string intention = name + std::string("_intention");

            Behaviors::Variable * statevar = 
                new Behaviors::Variable(state, Behaviors::t_bool);
            Behaviors::Variable * intentvar =
                new Behaviors::Variable(intention, Behaviors::t_bool);


            _state_variables.insert( 
                    std::pair< Architecture::NetworkComponent *,
                    Behaviors::Variable * >(curr, statevar));

            _contract->environment_variables.push_back(statevar);

            _intention_variables.insert( 
                    std::pair< Architecture::NetworkComponent *,
                    Behaviors::Variable * >(curr, intentvar));

            _contract->system_variables.push_back(intentvar);

        }
    }
}

    Behaviors::Variable * 
Problem::getIntentionVariable( Architecture::NetworkComponent * c )
{
    std::map< 
        Architecture::NetworkComponent *, 
        Behaviors::Variable * >::iterator it;
    it = _intention_variables.find(c);
    if( it != _intention_variables.end() )
    {
        return (*it).second;
    } 
    else 
    {
        return NULL;
    }
}

    Behaviors::Variable * 
Problem::getStateVariable( Architecture::NetworkComponent * c )
{
    std::map< 
        Architecture::NetworkComponent *, 
        Behaviors::Variable * >::iterator it;
    it = _state_variables.find(c);
    if( it != _state_variables.end() )
    {
        std::pair< 
            Architecture::NetworkComponent *,
            Behaviors::Variable * > p = (*it);
        return p.second;
    } 
    else 
    {
        return NULL;
    }
}

Contracts::AGContract * Problem::finalizeContract()
{
    std::list< Architecture::NetworkComponent * >::iterator it;

    for( it = _network->switches.begin();
            it != _network->switches.end();
            ++it )
    {
        Architecture::NetworkComponent * currcomp = *it;
        if( constant_components.find(currcomp) == constant_components.end() ||
                noncontrollable_components.find(currcomp) == 
                noncontrollable_components.end() )
        {

            if( physical_switch_on.find(currcomp) == physical_switch_on.end() )
            {

                Behaviors::Logics::LogicProposition * intent;
                Behaviors::Logics::LogicProposition * state;


                intent = new Behaviors::Logics::LogicProposition(
                        getIntentionVariable(currcomp)->GetName());

                state = new Behaviors::Logics::LogicProposition(
                        getStateVariable(currcomp)->GetName());

                Behaviors::Logics::WellFormedFormula * assumptions =
                    static_cast< Behaviors::Logics::WellFormedFormula * >(
                            _contract->assumptions );

                _contract->assumptions = 
                    static_cast< Behaviors::Logics::WellFormedFormula * >(
                            new BinaryLogicCombination(
                                op_land,
                                assumptions,
                                new UnaryTemporalFormula( op_globally,
                                    new Behaviors::Logics::BinaryLogicCombination(
                                        op_implies,
                                        intent,
                                        new Behaviors::Logics::UnaryTemporalFormula(
                                            op_next,
                                            state)))));
            }

            if( physical_switch_off.find(currcomp) == physical_switch_off.end())
            {

                Behaviors::Logics::LogicProposition * intent;
                Behaviors::Logics::LogicProposition * state;


                intent = new Behaviors::Logics::LogicProposition(
                        getIntentionVariable(currcomp)->GetName());

                state = new Behaviors::Logics::LogicProposition(
                        getStateVariable(currcomp)->GetName());

                Behaviors::Logics::WellFormedFormula * assumptions =
                    static_cast< Behaviors::Logics::WellFormedFormula * >(
                            _contract->assumptions );

                Behaviors::Logics::WellFormedFormula * nintent;
                Behaviors::Logics::WellFormedFormula * nstate;

                nintent = new Behaviors::Logics::UnaryLogicCombination(
                        op_lnot,
                        intent );
                nstate = new Behaviors::Logics::UnaryLogicCombination(
                        op_lnot,
                        state );

                _contract->assumptions = 
                    static_cast< Behaviors::Logics::WellFormedFormula * >(
                            new BinaryLogicCombination(
                                op_land,
                                assumptions,
                                new UnaryTemporalFormula( op_globally,
                                    new Behaviors::Logics::BinaryLogicCombination(
                                        op_implies,
                                        nintent,
                                        new Behaviors::Logics::UnaryTemporalFormula(
                                            op_next,
                                            nstate)))));
            }
        }
    }

    for( it = _network->components.begin();
            it != _network->components.end();
            ++it )
    {        
        Architecture::NetworkComponent * currcomp = *it;

        if( constant_components.find(currcomp) != 
                constant_components.end() )
        {
            Behaviors::Variable * var = getStateVariable(currcomp);
            std::list< Behaviors::Variable * >::iterator varit;
            for( varit = _contract->environment_variables.begin();
                    varit != _contract->environment_variables.end();
                    ++varit )
            {
                if( var == *varit )
                {
                    _contract->environment_variables.erase(varit);
                    break;
                }
            }
        }
        if( noncontrollable_components.find(currcomp) != 
                noncontrollable_components.end() )
        {
            Behaviors::Variable * var = getIntentionVariable(currcomp);
            std::list< Behaviors::Variable * >::iterator varit;
            for( varit = _contract->system_variables.begin();
                    varit != _contract->system_variables.end();
                    ++varit )
            {
                if( var == *varit )
                {
                    _contract->system_variables.erase(varit);
                    break;
                }
            }

        }


    }

    return _contract;


}


Contracts::AGContract * Problem::getContract()
{

    // Implement transitions between intention

    return _contract;
}


void Problem::setTimeResolution( Behaviors::Timing::chase_time t )
{
    _resolution = t;
}

Behaviors::Timing::chase_time Problem::getTimeResolution()
{
    return _resolution;
}




