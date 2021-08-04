/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/02/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "DSLParser/DesignProblem.hh"
#include "utilities/GraphUtilities.hh"

using namespace chase;
using namespace patternsOnNetworks;

LogicFormula * DesignProblem::_activatePath( std::vector<unsigned>& path)
{
    if( path.size() < 1 )
        messageError("Empty path.");

    LogicFormula * ret;

    size_t i = 0;
    std::string comp_name;

    // Find the first element controllable.
    std::map< std::string, Variable * >::iterator found;
    while(i < path.size())
    {
        comp_name =
                _architecture->getVertex(path[i])->getName()->getString();
        found = _commandVariables.find(comp_name);
        ++i;
        if( found != _commandVariables.end() ) break;
    }
    if(found == _commandVariables.end() )
        messageError("Empty path.");

    ret = Prop(found->second);

    while( i < path.size() )
    {
        comp_name =
                _architecture->getVertex(path[i])->getName()->getString();
        found = _commandVariables.find(comp_name);
        ++i;
        if(found == _commandVariables.end()) continue;

        // Else
        ret = And(ret, Prop(found->second));
    }
    return ret;
}

LogicFormula * DesignProblem::_pathDoesExist(std::vector<unsigned> &path)
{
    if( path.size() < 1 )
        messageError("Empty path.");

    LogicFormula * ret;

    size_t i = 0;
    std::string comp_name;

    // Find the first element that has a state variable.
    std::map< std::string, Variable * >::iterator found;
    while( i < path.size() )
    {
        comp_name =
                _architecture->getVertex(path[i])->getName()->getString();
        found = _stateVariables.find(comp_name);
        ++i;
        if( found != _stateVariables.end() ) break;
    }
    if(found == _stateVariables.end() )
        messageError("Empty path.");

    ret = Prop(found->second);

    while( i < path.size() )
    {
        comp_name =
                _architecture->getVertex(path[i])->getName()->getString();
        found = _stateVariables.find(comp_name);
        ++i;
        if(found == _stateVariables.end()) continue;

        // Else
        ret = And(ret, Prop(found->second));
    }
    return ret;
}
