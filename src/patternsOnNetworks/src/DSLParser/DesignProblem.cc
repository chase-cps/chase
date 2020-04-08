/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include <algorithm>
#include <cmath>
#include "DSLParser/DesignProblem.hh"

using namespace chase;
using namespace patternsOnNetworks;

DesignProblem::DesignProblem() :
    _contract(nullptr),
    _precision(),
    _architecture(nullptr)
{
}

DesignProblem::~DesignProblem() = default;

Contract * DesignProblem::getContract()
{
    if( _contract ) return _contract;
    else return _generateContract();
}

Contract * DesignProblem::_generateContract()
{
    _contract = new Contract();

    // ---------------------------------------------------------------------- //
    // -------------------- Guarantees -------------------------------------- //
    // ---------------------------------------------------------------------- //

    // Manage the assumptions. It must prune the state variables.
    _pruneComponentsVariables();

    // Generate the components variables.
    _generateComponentsVariables();

    // Create the graph.
    _createArchitecture();

    // Retrieve the timing precision by analyzing all the requirements.
    _retrieveTimingPrecision();

    // Analyzes and build the assumptions.
    _analyzeAssumptions();

    // Complete the mechanism command -> state for the components without a
    // specific actuation delay.
    _completeCommandState();

    // ---------------------------------------------------------------------- //
    // -------------------- Guarantees -------------------------------------- //
    // ---------------------------------------------------------------------- //

    // Analyzes and build the assumptions.
    _analyzeRequirements();


    // Perform Union of all the sets (ENV, SYS, INIT) for assumptions
    std::set< std::shared_ptr< chase::LogicFormula > > assump;

    assump.insert(_gr1_env_init.begin(), _gr1_env_init.end());
    assump.insert(_gr1_env_safety.begin(), _gr1_env_safety.end());
    assump.insert(_gr1_env_liveness.begin(), _gr1_env_liveness.end());

    if(! assump.empty() ) {
        std::shared_ptr< LargeBooleanFormula > large(new
                LargeBooleanFormula());
        for (auto f : assump) {
            large->addOperand(std::shared_ptr<LogicFormula>(f));
        }
        _contract->addAssumptions(logic, large);
    }

    // Perform Union of all the sets (ENV, SYS, INIT) for Guarantees.
    std::set< std::shared_ptr< chase::LogicFormula > > guarant;

    guarant.insert(_gr1_sys_init.begin(), _gr1_sys_init.end());
    guarant.insert(_gr1_sys_safety.begin(), _gr1_sys_safety.end());
    guarant.insert(_gr1_sys_liveness.begin(), _gr1_sys_liveness.end());

    if(! guarant.empty() ) {
        std::shared_ptr< LargeBooleanFormula > large(
                new LargeBooleanFormula());
        for (auto f : guarant) {
            large->addOperand(std::shared_ptr<LogicFormula>(f));
        }
        _contract->addGuarantees(logic, large);
    }

    return _contract;
}

void DesignProblem::_createArchitecture() {
    unsigned int nodes = components.size();
    _architecture =
            std::make_shared<Graph>(nodes, false, Nam("architecture"));



    // Create the vertexes.
    std::map<std::string, Component *>::iterator cit;
    unsigned int node_index = 0;
    for (cit = components.begin(); cit != components.end(); ++cit) {
        std::string node_name((*cit).first);
        std::shared_ptr<Vertex> v(new Vertex(Nam(node_name)));
        _architecture->associateVertex(node_index, v);
        ++node_index;
    }

    // Create the edges.
    // - Unswitched connections.
    std::set<Connection *>::iterator uit;
    for (uit = unswitched_connections.begin();
         uit != unswitched_connections.end(); ++uit) {
        Connection *current = *uit;
        std::string source = current->getSourceName();
        std::string target = current->getTargetName();

        int source_id = _architecture->getVertexIndex(source);
        if (source_id < 0) messageError("Vertex not found: " + source);
        int target_id = _architecture->getVertexIndex(target);
        if (target_id < 0) messageError("Vertex not found: " + target);

        std::shared_ptr< Edge > edge(new Edge(source_id, target_id));

        _architecture->addEdge(edge);
    }

    // Create the edges.
    // - Switched connections.
    std::set<SwitchedConnection *>::iterator sit;
    for (sit = switched_connections.begin();
         sit != switched_connections.end(); ++sit)
    {
        SwitchedConnection * current = *sit;
        std::string source = current->getSourceName();
        std::string target = current->getTargetName();
        std::string sw = current->getSwitch()->name;

        int source_id = _architecture->getVertexIndex(source);
        if (source_id < 0) messageError("Vertex not found: " + source);
        int target_id = _architecture->getVertexIndex(target);
        if (target_id < 0) messageError("Vertex not found: " + target);
        int sw_id = _architecture->getVertexIndex(sw);
        if (sw_id < 0) messageError("Vertex not found: " + sw);


        std::shared_ptr< Edge > edge_1(new Edge(source_id, sw_id ));
        std::shared_ptr< Edge > edge_2(new Edge(sw_id, target_id ));

        _architecture->addEdge(edge_1);
        _architecture->addEdge(edge_2);
    }

    // add the graph to the contract.
    std::pair< semantic_domain , std::shared_ptr<Graph> >
            p(graph, _architecture);
    _architecture->setParent(_contract);
    _contract->assumptions.insert(p);

}

void DesignProblem::_pruneComponentsVariables()
{
    _findNonVariableComponents();
    _findNonControllableComponents();
}

void DesignProblem::_findNonVariableComponents()
{
    std::set< SpecFunction * >::iterator ait;

    // Find the non variable components.
    // - search for the no-failure pattern.
    std::string pattern("no-failures");
    for( ait = assumptions.begin(); ait != assumptions.end(); ++ait )
    {
        SpecFunction * assumption = (*ait);
        if( assumption->name != pattern ) continue;

        std::string param = assumption->parameters[0];

        std::set< std::string > comps = _findComponents( param );
        // Perform the union of the _nonVariableComponents & components sets.
        _nonVariableComponents.insert(comps.begin(), comps.end());
    }
}

void DesignProblem::_findNonControllableComponents()
{
    std::set< SpecFunction * >::iterator ait;

    // Find the non variable components.
    // - search for the no-control pattern.
    std::string pattern("no-control");
    for( ait = assumptions.begin(); ait != assumptions.end(); ++ait )
    {
        SpecFunction * assumption = (*ait);
        if( assumption->name != pattern ) continue;

        std::string param = assumption->parameters[0];

        std::set< std::string > comps = _findComponents( param );
        // Perform the union of the _nonControllableComponents & components sets.
        _nonControllableComponents.insert(comps.begin(), comps.end());
    }
}

std::set< std::string > DesignProblem::_findComponents( std::string s ) {
    std::set<std::string> ret;

    // Check if the string refers to a single component.
    std::map<std::string, Component *>::iterator cit;
    cit = components.find(s);
    if (cit != components.end())
    {
        // The string represent a single component.
        ret.insert((*cit).first);
        return ret;
    }

    // Check if the string refers to a abbreviation of a single component.
    std::map< std::string, std::string >::iterator ait;
    ait = abbreviations.find(s);
    if (ait != abbreviations.end())
    {
        // The string represent the abbreviation of a single component.
        ret.insert((*ait).second);
        return ret;
    }

    // Check if the string refers to a type.
    std::map< std::string, DomainSpecificType * >::iterator tit;
    tit = domain_specific_types.find(s);
    if( tit != domain_specific_types.end() )
    {
        DomainSpecificType * dst = tit->second;
        for(cit = components.begin(); cit != components.end(); ++cit)
        {
            if(cit->second->type == dst)
                ret.insert(cit->first);
        }
        return ret;
    }

    // Check if the string refers to a group of components.
    std::map< std::string, ComponentsGroup * >::iterator git;
    git = components_groups.find(s);
    if( git != components_groups.end() )
    {
        ComponentsGroup * group = git->second;
        std::set< Component * >::iterator it;
        for(it = group->components.begin(); it != group->components.end(); ++it)
        {
            Component * c = *it;
            ret.insert(c->name);
        }
        return ret;
    }
    return ret;
}

void DesignProblem::_generateComponentsVariables()
{
    std::map< std::string, Component * >::iterator cit;
    for( cit = components.begin(); cit != components.end(); ++cit )
    {
        std::string component_name = cit->first;

        if( _nonVariableComponents.find(component_name)
                    == _nonVariableComponents.end() )
        {
            std::string var_name(component_name);
            std::replace(var_name.begin(), var_name.end(), ' ', '_');
            var_name += "_state";

            std::shared_ptr< Variable > state_var(new Variable(
                    Bool(),
                    Nam(var_name),
                    input));

            std::pair< std::string, std::shared_ptr<Variable> >
                    p(component_name,state_var);

            _stateVariables.insert(p);

            _contract->declarations.push_back(state_var);
        }

        if( _nonControllableComponents.find(component_name)
            == _nonControllableComponents.end() )
        {
            std::string var_name(component_name);
            std::replace(var_name.begin(), var_name.end(), ' ', '_');
            var_name += "_command";

            std::shared_ptr< Variable > comm_var (new Variable(
                    Bool(),
                    Nam(var_name),
                    output));

            std::pair< std::string, std::shared_ptr<Variable> >
                    p(component_name, comm_var);

            _commandVariables.insert(p);

            _contract->declarations.push_back(comm_var);
        }
    }
}

chase::chase_time *
DesignProblem::_compute_precision(std::vector<chase_time *> * timings) {

    // Create the set to compute the GCD.
    std::set< unsigned int > normalized;
    for(auto & timing : *timings)
    {
        normalized.insert(
                timing->amount * pow(10, 3*timing->unit));
    }

    std::vector< unsigned int > values(normalized.size());
    std::copy(normalized.begin(), normalized.end(), values.begin());
    // Compute the CGD.
    unsigned int GCD;

    if( values.empty() ) return new chase::chase_time(1, CHASE_SEC);
    if(values.size() == 1 ) GCD = values[0];
    else
    {
        unsigned int x = values[0];
        unsigned int y = values[1];

        GCD = x;
        for( unsigned int i = 1; i < values.size(); ++i )
        {
            unsigned int a = std::max(x,y);
            unsigned int b = std::min(x,y);
            unsigned int c = 1;

            do{
                c = a%b;
                GCD = b;
                a = b;
                b = c;
            } while( c!= 0 );
            x = GCD;
            y = values[i + 1];
        }
    }

    // Find the unit.
    unsigned int unit;
    for(unit = CHASE_SEC; unit <= CHASE_FS; ++unit )
    {
        unsigned int remainder =
                GCD % static_cast<unsigned int>(pow(10, 3 * unit));
        if( remainder != 0 )
        {
            break;
        }
        else GCD /= 1000;
    }

    return new chase_time(GCD, static_cast<chase_time_unit >(unit));
}

void DesignProblem::_retrieveTimingPrecision()
{
    std::vector< chase_time * > timings;
    std::set< SpecFunction * >::iterator spec_it;
    // Check the timing in the assumptions.
    for(spec_it = assumptions.begin(); spec_it != assumptions.end(); ++spec_it)
    {
        // The assumptions patterns impacting on timing are:
        // - switch-on-time,
        // - switch-off-time
        SpecFunction * curr = *spec_it;
        std::string spec = curr->name;
        if( spec == "switch-on-time" || spec == "switch-off-time")
        {
            timings.push_back(
                    new chase_time(
                            atoi(curr->parameters[1].c_str()),
                            curr->parameters[2])
                    );
        }
    }

    // Check the timing in the guarantees.
    for(spec_it = requirements.begin();
        spec_it != requirements.end(); ++spec_it)
    {
        // The guarantees patterns impacting on timing are:
        // - must-disconnect-failed,
        // - never-disconnect
        SpecFunction * curr = *spec_it;
        std::string spec = curr->name;
        if( spec == "must-disconnect-failed" )
        {
            timings.push_back(
                    new chase_time(
                            atoi(curr->parameters[1].c_str()),
                            curr->parameters[2])
            );
        }
        if( spec == "never-disconnect" )
        {
            timings.push_back(
                    new chase_time(
                            atoi(curr->parameters[2].c_str()),
                            curr->parameters[3])
            );
        }
    }

    _precision = _compute_precision( & timings );
}

void DesignProblem::_analyzeAssumptions()
{
    std::set< SpecFunction * >::iterator it;
    for(it = assumptions.begin(); it != assumptions.end(); ++it) {
        SpecFunction *spec = *it;
        if (spec->name == "no-recovery")
            _noRecovery(spec);
        if (spec->name == "max-failures")
            _maxFailures(spec);
        if (spec->name == "initial-state")
            _initialState(spec);
        if (spec->name == "switch-on-time")
            _switchTime(spec, true);
        if (spec->name == "switch-off-time")
            _switchTime(spec, false);
    }
}

void DesignProblem::_completeCommandState()
{
    for( auto cit = components.begin(); cit != components.end(); ++cit)
    {
        std::string comp_name = (*cit).first;

        auto state = _stateVariables.find(comp_name)->second;
        auto command = _commandVariables.find(comp_name)->second;

        auto found = _offTimer.find(comp_name);
        auto controllable = _commandVariables.find(comp_name);

        if(found == _offTimer.end() && controllable != _commandVariables.end())
        {
            auto state_prop = Prop(state);
            auto command_prop = Prop(command);

            auto formula = Always(
                    Implies(
                            Not(command_prop),
                            Next(Not(state_prop))));

            _gr1_env_safety.insert(formula);
        }

        found = _onTimer.find(comp_name);
        if( found == _onTimer.end() && controllable != _commandVariables.end())
        {
            auto state_prop = Prop(state);
            auto command_prop = Prop(command);

            auto formula = Always(Implies(command_prop, Next(state_prop)));

            _gr1_env_safety.insert(formula);
        }
    }
}

void DesignProblem::_analyzeRequirements() {
    std::set< SpecFunction * >::iterator it;
    for(it = requirements.begin(); it != requirements.end(); ++it) {
        SpecFunction *spec = *it;
        if (spec->name == "never-connect")
            _neverConnect(spec);
        if (spec->name == "must-disconnect-failed")
            _mustDisconnectFailed(spec);
        if (spec->name == "prefer-active-connection")
            _preferActiveConnection(spec);
        if (spec->name == "never-disconnect")
            _neverDisconnect(spec);
        if (spec->name == "keep-connection-stable")
            _keepConnectionStable(spec);
    }
}

unsigned int DesignProblem::_compute_steps(chase::chase_time &timing) {
    unsigned int resolution = _precision->amount;
    resolution *= pow(10, _precision->unit * 3);

    unsigned int normalized = timing.amount;
    normalized *= pow(10, timing.unit * 3);

    unsigned int steps = normalized / resolution;
    return steps;
}















