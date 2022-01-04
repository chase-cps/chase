/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/3/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/FunctionCall.hh"
#include "chase/representation/Function.hh"

using namespace chase;

FunctionCall::FunctionCall() :
    Value()
{
    _node_type = functionaCall_node;
}

FunctionCall::~FunctionCall() = default;

int FunctionCall::accept_visitor(BaseVisitor &v) {
    return v.visitFunctionCall(*this);
}

std::string FunctionCall::getString() {
    std::string ret("");
    return ret;
}

FunctionCall *FunctionCall::clone() {
    auto ret = new FunctionCall();
    return ret;
}

Function *FunctionCall::getFunction() const {
    return _function;
}

void FunctionCall::setFunction(Function *function, bool initialize) {
    _function = function;
    if(initialize || _parameters.empty())
        for(size_t i = 0; i < function->getArity(); ++i)
            _parameters[i] = nullptr;
}

Value *FunctionCall::parameter(size_t i, Value *value) {
    if(value != nullptr)
    {
        if(i >= _function->getArity()) {
            messageWarning("The index is greater the function arity.");
            return nullptr;
        }
        _parameters[i] = value;
    }
    return _parameters[i];
}
