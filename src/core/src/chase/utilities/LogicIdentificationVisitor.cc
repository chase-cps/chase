/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        8/24/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/utilities/LogicIdentificationVisitor.hh"
#include "representation.hh"

using namespace chase;

LogicIdentificationVisitor::LogicIdentificationVisitor() :
    GuideVisitor(0),
    _isLogics(false),
    _isTemporal(false),
    _hasReal(false),
    _hasIntervals(false)
{
}

logics_type LogicIdentificationVisitor::identifyContractType( Contract * contract )
{
    _isLogics = false;
    _isTemporal = false;
    _hasReal = false;
    _hasIntervals = false;

    contract->accept_visitor(*this);

    if(!_isLogics) return logics_type::no_logics;
    if(!_isTemporal) {
        return logics_type::propositional;
    } else {
        if(_hasReal && _hasIntervals) return logics_type::signal_temporal;
        else if(_hasIntervals) return logics_type::metric_temporal;
        else return logics_type::linear_temporal;
    }
}

int LogicIdentificationVisitor::visitRealValue(RealValue &value) {
    _hasReal = true;
    return GuideVisitor::visitRealValue(value);
}

int LogicIdentificationVisitor::visitInterval(Interval &interval) {
    _hasIntervals = true;
    return GuideVisitor::visitInterval(interval);
}

int LogicIdentificationVisitor::visitVariable(Variable &variable) {
    if(variable.getType()->IsA() == real_node) _hasReal = true;
    return GuideVisitor::visitVariable(variable);
}

int LogicIdentificationVisitor::visitConstant(Constant &constant) {
    if(constant.getType()->IsA() == real_node) _hasReal = true;
    return GuideVisitor::visitConstant(constant);
}

int LogicIdentificationVisitor::visitUnaryTemporalOperation(
        UnaryTemporalFormula &formula) {
    _isTemporal = true;
    return GuideVisitor::visitUnaryTemporalOperation(formula);
}

int LogicIdentificationVisitor::visitBinaryTemporalOperation(
        BinaryTemporalFormula &formula) {
    _isTemporal = true;
    return GuideVisitor::visitBinaryTemporalOperation(formula);
}

int LogicIdentificationVisitor::visitContract(Contract &contract) {
    if(contract.assumptions.find(logic) != contract.assumptions.end())
        _isLogics = true;
    if(contract.guarantees.find(logic) != contract.guarantees.end())
        _isLogics = true;

    return GuideVisitor::visitContract(contract);
}

int LogicIdentificationVisitor::visitLargeBooleanFormula(LargeBooleanFormula &formula) {
    _isLogics = true;
    return GuideVisitor::visitLargeBooleanFormula(formula);
}


LogicIdentificationVisitor::~LogicIdentificationVisitor() = default;
