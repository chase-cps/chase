/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "DSLSpecsBuilder.hh"


using namespace DSLFrontend;
using namespace chase;
using namespace antlr4;

DSLSpecsBuilder::DSLSpecsBuilder() :
    _inConnections(false),
    _inSwitched(false),
    _inUnswitched(false),
    _currentSwitchType(nullptr),
    _assumptions(false),
    _requirements(false)
{
    _problem = new Problem();
}

DSLSpecsBuilder::~DSLSpecsBuilder() {
    delete _problem;
}

Problem *DSLSpecsBuilder::getProblem() {
    return _problem;
}

Contract * DSLSpecsBuilder::getContract() {
    if(_problem != nullptr) return _problem->getContract();
    else return nullptr;
}

Problem *DSLSpecsBuilder::parseSpecificationFile(std::string infile) {
    ANTLRFileStream input( infile );
    ChaseLexer lexer( &input );
    CommonTokenStream tokens( &lexer );
    ChaseParser parser( &tokens );

    tree::ParseTree * tree = parser.chaseSpec();

    auto walker = new tree::ParseTreeWalker();
    walker->walk(this, tree);

    return _problem;
}


void DSLSpecsBuilder::enterType(ChaseParser::TypeContext *context) {
    std::string name = _getNameFromContext(context->nam);

    component_type type;
    if (context->typ != nullptr) {
        std::string t = context->typ->getText();
        type = _resolveType(t);
        if (type == ct_error)
            messageError(std::string("Wrong type: ") + t);
    } else {
        messageError(std::string("Unspecified abstract type for ") + name);
    }

    // Create the type.
    auto dst = new DomainSpecificType(name, type);
    std::vector<Token *>::iterator pit;
    unsigned int counter = 0;
    for (pit = context->par.begin(); pit != context->par.end(); ++pit)
    {
        Token * tok = *pit;
        std::string param(tok->getText());
        if(dst->n_flow_parameters > counter) {
            dst->flow_parameters[counter] = param;
            ++counter;
        }
        else
            messageError(
                    std::string("Too many parameters for type: "
                    + name + ", Maximum:" +
                    std::to_string(dst->n_flow_parameters))
                    );
    }
    std::pair< std::string, DomainSpecificType * > p(name, dst);
    _problem->domain_specific_types.insert(p);
}

void DSLSpecsBuilder::enterComponent(ChaseParser::ComponentContext *context) {
    // Retrieve the name of the group of components.
    std::string declaration = _getNameFromContext( context->nam );

    // Retrieve the number of components.
    unsigned int number = 0;
    if( context->num != nullptr )
    {
        std::string num = context->num->getText();
        number = atoi(num.c_str());

    }

    // First the procedure checks whether the declaration specify a sub-group.
    ComponentsGroup * group;

    std::map< std::string, DomainSpecificType * >::iterator mit;
    mit = _problem->domain_specific_types.find(declaration);
    if( mit != _problem->domain_specific_types.end() )
    {
         group = new ComponentsGroup( "", (*mit).second, number );
    }
    else
    {
        // Find the type associated with the subtype: as a convention, the name of
        // the component groups should be in the form:
        //          attribute type: num_of_components
        // the components generated are named: attribute type num.
        size_t blank = declaration.find(' ');
        std::string attribute = declaration.substr(0, blank);
        std::string type = declaration.substr(blank + 1);

        mit = _problem->domain_specific_types.find(type);
        group = new ComponentsGroup( attribute, (*mit).second, number );
    }

    // Insert the components of the group in the list of components of the
    // design problem.
    std::set< Component * >::iterator sit;
    for( sit = group->components.begin();
        sit != group->components.end(); ++sit )
    {
        Component * c = *sit;
        std::pair< std::string, Component * > p(c->name, c);
        _problem->components.insert(p);
    }

    // Insert the group in the list of groups of the design problem.
    std::pair< std::string, ComponentsGroup * > p(declaration, group);
    _problem->components_groups.insert(p);
}

void DSLSpecsBuilder::enterAbbrev(ChaseParser::AbbrevContext *context) {
    if( context->abbr == nullptr ) return;

    std::string abbrev = context->abbr->getText();
    std::string name = _getNameFromContext( context->nam );

    auto it = _problem->components.find(name);
    if(it == _problem->components.end())
        messageWarning(
                std::string("Impossible to find component: ") + name);

    // Insert the abbreviation in the object.
    Component * comp = (*it).second;
    comp->abbreviation = abbrev;

    // Insert the abbreviation in the map.
    _problem->abbreviations.insert(
            std::pair<std::string, std::string>(abbrev, name));
}

void DSLSpecsBuilder::enterConnections(ChaseParser::ConnectionsContext * )
{
    _inConnections = true;
    _inSwitched = false;
    _inUnswitched = false;
}

void DSLSpecsBuilder::exitConnections(ChaseParser::ConnectionsContext * )
{
    _inConnections = false;
    _inSwitched = false;
    _inUnswitched = false;
}

void DSLSpecsBuilder::enterSwitched(ChaseParser::SwitchedContext * context )
{
    _inSwitched = true;
    _inUnswitched = false;

    // retrieve the type of switch being used.
    std::string sw = _getNameFromContext(context->sw);

    // Find the switch type.
    std::map< std::string, DomainSpecificType * >::iterator it;
    it = _problem->domain_specific_types.find(sw);

    // If found, set the current switch type to continue parsing.
    if( it != _problem->domain_specific_types.end() )
    {
        _currentSwitchType = (*it).second;
    }
    else
        // Else, error.
        messageError("Wrong switch type: " + sw);
}

void DSLSpecsBuilder::enterUnswitched(ChaseParser::UnswitchedContext * )
{
    _inSwitched = false;
    _inUnswitched = true;
}


void DSLSpecsBuilder::enterConn(ChaseParser::ConnContext * context ) {
    if( ! _inConnections )
        messageError(
                "Wrong format: connection declared out of connections section.");

    if( _inSwitched && ! _inUnswitched ) _enterSwitchedConn(context);
    else if ( !_inSwitched && _inUnswitched ) _enterUnswitchedConn(context);
    else if ( !_inSwitched && !_inUnswitched ) _enterSpecificConn(context);
    else
        messageError(
                "Wrong format of connection declaration.");
}

void DSLSpecsBuilder::_enterSwitchedConn(ChaseParser::ConnContext *ctx)
{
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);

    Component * component_from = _getComponent(from);
    Component * component_to = _getComponent(to);

    if(component_from == nullptr)
        messageError("Component does not exist: " + from);
    if(component_to == nullptr)
        messageError("Component does not exist: " + to);

    auto connection = new SwitchedConnection(
            component_from, component_to, _currentSwitchType );

    _problem->switched_connections.insert(connection);
}

void DSLSpecsBuilder::_enterUnswitchedConn(ChaseParser::ConnContext *ctx)
{
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);

    Component * component_from = _getComponent(from);
    Component * component_to = _getComponent(to);

    if(component_from == nullptr)
        messageError("Component does not exist: " + from);
    if(component_to == nullptr)
        messageError("Component does not exist: " + to);

    auto connection = new Connection(component_from, component_to);

    _problem->unswitched_connections.insert(connection);
}

void DSLSpecsBuilder::_enterSpecificConn(ChaseParser::ConnContext *ctx)
{
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);
    std::string sw = _getNameFromContext(ctx->sw);

    Component * component_from = _getComponent(from);
    Component * component_to = _getComponent(to);
    Component * component_sw = _getComponent(sw);

    if(component_from == nullptr)
        messageError("Component does not exist: " + from);
    if(component_to == nullptr)
        messageError("Component does not exist: " + to);

    if(component_sw == nullptr)
    {
        size_t pos = sw.find(' ');
        std::string t = sw.substr(0, pos);
        std::map< std::string, DomainSpecificType * >::iterator it;
        it = _problem->domain_specific_types.find(t);

        // Create a component with the given name, and the found type.
        component_sw = new Component(sw, (*it).second);
    }

    auto connection = new SwitchedConnection(
            component_from, component_to, component_sw->type, component_sw );

    _problem->switched_connections.insert(connection);
    _problem->components.insert(
            std::pair< std::string, Component * >(
                    component_sw->name, component_sw));
}

void DSLSpecsBuilder::enterRequirements(ChaseParser::RequirementsContext * ) {
    _requirements = true;
}

void DSLSpecsBuilder::exitRequirements(ChaseParser::RequirementsContext * ) {
    _requirements = false;
}

void DSLSpecsBuilder::enterAssumptions(ChaseParser::AssumptionsContext * ) {
    _assumptions = true;
}

void DSLSpecsBuilder::exitAssumptions(ChaseParser::AssumptionsContext * ) {
    _assumptions = false;
}

void DSLSpecsBuilder::enterReq(ChaseParser::ReqContext *context) {
    std::string req = context->pred->getText();
    auto function = new SpecFunction(req);
    _currentFunction = function;

    if(_assumptions) _problem->assumptions.insert(function);
    if(_requirements) _problem->requirements.insert(function);
}

void DSLSpecsBuilder::enterParam(ChaseParser::ParamContext *context) {
    if( context->num != nullptr )
        _currentFunction->parameters.push_back(context->num->getText());
    if( context->nam != nullptr )
    {
        // Use the _getNameFromContext to avoid the insertion of commas.
        _currentFunction->parameters.push_back(
                _getNameFromContext(context->nam));
    }
}
