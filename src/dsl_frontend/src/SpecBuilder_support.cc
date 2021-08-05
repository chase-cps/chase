/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "DSLSpecsBuilder.hh"

using namespace DSLFrontend;
using namespace antlr4;

std::string
DSLSpecsBuilder::_getNameFromContext(ChaseParser::NameContext *context) {
    std::string ret;


    /// \todo Find a way to fix the following stupid line.
    // This correction is due to a RIDICOLOUS choice by the ANTLR4 team.
    if(context->num != nullptr) context->comp.push_back(context->num);

    std::vector< Token * >::iterator it;
    for(it = context->comp.begin(); it != context->comp.end(); ++it)
    {
        Token * tok = *it;
        ret += std::string(tok->getText());
        ret += std::string(" ");
    }
    ret = ret.substr(0,ret.size()-1);
    return ret;
}

std::string
DSLSpecsBuilder::_getNameFromContext(Ref<ChaseParser::NameContext> context)
{
    std::string ret = _getNameFromContext(context.get() );
    if( context->num != nullptr )
    {
        std::string num = context->num->getText();
        ret += std::string(" ");
        ret += num;
    }
    return ret;
}

component_type DSLSpecsBuilder::_resolveType(std::string t) {
    if(t == "Generic") return ct_generic;
    if(t == "Source") return ct_source;
    if(t == "Bus") return ct_bus;
    if(t == "Converter") return ct_converter;
    if(t == "Sink") return ct_sink;
    if(t == "Switch") return ct_switch;
    else return ct_error;
}

Component *DSLSpecsBuilder::_getComponent(std::string str)
{
    auto by_abbrev = _problem->abbreviations.find(str);
    if( by_abbrev != _problem->abbreviations.end() )
        str = (*by_abbrev).second;

    auto by_name = _problem->components.find(str);
    if( by_name != _problem->components.end() )
        return (*by_name).second;

    return nullptr;
}
