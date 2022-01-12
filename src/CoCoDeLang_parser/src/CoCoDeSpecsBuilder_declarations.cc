/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/14/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "CoCoDeSpecsBuilder.hh"

using namespace chase;
using namespace antlr4;

antlrcpp::Any CoCoDeSpecsBuilder::visitConstant(
        CoCoDeLangParser::ConstantContext *ctx)
{
    /// \todo In constant creation, manage the domain in case the value of the
    /// constant is out of the standard domain.
    auto type = analyzeBaseType(ctx->basetype());
    std::string id = ctx->ID()->getText();

    // Find the value.
    Value * value = nullptr;
    // In case the type is boolean.
    if(type->IsA() == boolean_node)
    {
        if(ctx->constant_definition()->trueKW() != nullptr)
            value = static_cast< Value* >(new BooleanValue(true));
        else if(ctx->constant_definition()->falseKW() != nullptr)
            value = static_cast< Value* >(new BooleanValue(false));
        else
            messageError(
                    "Invalid boolean constant: " +
                    ctx->constant_definition()->getText());
    }
    // In case the type is Real.
    if(type->IsA() == real_node)
    {
        double val =
                atof(ctx->constant_definition()->number()->toString().c_str());
        value = static_cast< Value * >(new RealValue(val));
    }
    // In case the type is Integer.
    if(type->IsA() == integer_node)
    {
        int val =
                atoi(ctx->constant_definition()->number()->toString().c_str());
        value = static_cast< Value * >(new IntegerValue(val));
    }
    Constant * constant = new Constant(type, new Name(id), value);
    messageInfo("Created constant:" + id);
    return CoCoDeLangBaseVisitor::visitConstant(ctx);
}

antlrcpp::Any CoCoDeSpecsBuilder::visitVariable(
        CoCoDeLangParser::VariableContext *ctx) {
    return CoCoDeLangBaseVisitor::visitVariable(ctx);
}

antlrcpp::Any CoCoDeSpecsBuilder::visitDistribution(
        CoCoDeLangParser::DistributionContext *ctx) {
    return CoCoDeLangBaseVisitor::visitDistribution(ctx);
}

chase::Type *CoCoDeSpecsBuilder::analyzeBaseType(
        CoCoDeLangParser::BasetypeContext *ctx) {
   if(ctx->booleanKW() != nullptr) return new chase::Boolean();
   if(ctx->integerKW() != nullptr) return new chase::Integer();
   if(ctx->realKW() != nullptr) return new chase::Real();
   return nullptr;
}


