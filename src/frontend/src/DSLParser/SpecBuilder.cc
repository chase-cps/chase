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
 * @file	frontend/src/DSLParser/SpecBuilder.cc
 * @brief
 */



#include "DSLParser/SpecBuilder.hh"

using namespace antlr4;

////////////////////////////////////////////////////////////////////////////////
// Support methods
//
////////////////////////////////////////////////////////////////////////////////

std::string SpecBuilder::_gen_random( const int len, Architecture::Network * n ) 
{
    bool needed = true;
    std::string ret;
    while(needed)
    {
        std::string s;
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        for (int i = 0; i < len; ++i) {
            s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        s[len] = 0;

        ret = std::string(s);


        // Check if the name already exists in the network.
        bool found = false;
        std::list< Architecture::NetworkComponent * >::iterator it;
        for( it = n->components.begin();
                it != n->components.end();
                ++it )
        {
            Architecture::NetworkComponent * c = *it;
            std::string n = c->getName();
            if( n.find(ret) != std::string::npos )
            {
                found = true;
            }
        }
        for( it = n->switches.begin();
                it != n->switches.end();
                ++it )
        {
            Architecture::NetworkComponent * c = *it;
            std::string n = c->getName();
            if( n.find(ret) != std::string::npos )
            {
                found = true;
            }
        }

        if( ! found ) needed = false;

    }
    return ret;
}


// Retrieve only the non numerical part of a name.
std::string SpecBuilder::_getNameFromContext( ChaseParser::NameContext * nam )
{
    std::vector< Token * >::iterator itT;
    std::string toReturn("");
    for( itT = nam->comp.begin();
            itT != nam->comp.end();
            ++itT )
    {
        Token * tk = *itT;
        toReturn += std::string(tk->getText());
        toReturn += std::string(" ");
    }
    toReturn = toReturn.substr(0,toReturn.size()-1);
    return toReturn;
}

// Retrieve the entire name.
std::string SpecBuilder::_getNameFromContext( 
        Ref< ChaseParser::NameContext > name )
{
    std::string c = _getNameFromContext(name.get());
    if( name->num != nullptr )
    {
        std::string num = name->num->getText();
        c += std::string(" ");
        c += num;
    }
    return c;
}

Architecture::component_types SpecBuilder::_resolveType( std::string t )
{
    if( t == "Generic" )
        return Architecture::ct_generic;
    if( t == "Source" )
        return Architecture::ct_source;
    if( t == "Bus")
        return Architecture::ct_bus;
    if( t == "Converter" )
        return Architecture::ct_converter;
    if( t == "Sink" )
        return Architecture::ct_sink;
    if( t == "Switch" )
        return Architecture::ct_switch;
    else
        return Architecture::ct_error;
}

////////////////////////////////////////////////////////////////////////////////
// Mandatory stuff: constructor, destructors.. etc..
//
///////////////////////////////////////////////////////////////////////////////

SpecBuilder::SpecBuilder() :
    _problem(NULL),
    _network(NULL),
    _specFile(),
    _errors(),
    _inConnections(false),
    _inSwitched(false),
    _inUnswitched(false),
    _currSwitchType(NULL),
    _inAssumptions(false),
    _inRequirements(false),
    _currRequirement(NULL)
{
    _network = new Architecture::Network();
    _problem = new Specification::Problem();
    _problem->setNetwork(_network);
    _network->setParent( _problem );
}

SpecBuilder::~SpecBuilder()
{
}

Specification::Problem * SpecBuilder::getProblem()
{
    return _problem;
}

Specification::Problem * SpecBuilder::parseSpecFile( std::string specFile )
{
    _specFile = specFile;
    ANTLRFileStream input( _specFile );

    ChaseLexer lexer( &input );
    CommonTokenStream tokens( &lexer );
    ChaseParser parser( &tokens );
    Ref< tree::ParseTree > tree;
    tree = parser.chaseSpec();
    tree::ParseTreeWalker * walker = new tree::ParseTreeWalker();

    walker->walk( this, tree.get() );


    return _problem; 
}


std::list< std::string > SpecBuilder::getErrors()
{
    return _errors;
}


////////////////////////////////////////////////////////////////////////////////
// Walk methods
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Components, Groups and Types management.
//
////////////////////////////////////////////////////////////////////////////////


void SpecBuilder::enterType( ChaseParser::TypeContext * ctx )
{
    std::string name = _getNameFromContext( ctx->nam );
    Architecture::component_types bt;
    if(ctx->typ != nullptr)
    {
        std::string basetype = ctx->typ->getText();
        bt = _resolveType(basetype);
        if( bt == Architecture::ct_error )
        {
            std::string error("ERROR: Wrong type:\n");
            error += basetype;
            _errors.push_back(error);
        }
    } else {
        bt = Architecture::ct_generic;
    }
    Architecture::DomainSpecificType * dst = 
        new Architecture::DomainSpecificType( name, bt );

    /// Parse parameters Flow parameters.
    std::vector< Token * >::iterator pit;
    int i = 0;
    for(pit = ctx->par.begin();
            pit != ctx->par.end();
            ++pit )
    {
        Token * tk = *pit;
        Architecture::FlowParameter param 
            = static_cast< Architecture::FlowParameter >(
                    std::string(tk->getText()));
        bool res = dst->addParameter(i, param);
        if( !res )
        {
            std::string error("ERROR: to many parameters in declaration of:\n");
            error += std::string(name);
            _errors.push_back(error);
        }
        ++i;
    }
    _network->types.push_front(dst);
}   

void SpecBuilder::enterComponent( ChaseParser::ComponentContext * ctx )
{
    /// \todo Find out why, passing the Ref< NameContext > it doesn't work.
    std::string name = _getNameFromContext( ctx->nam );

    // Retrieve number of components.
    unsigned int number = 0;
    if( ctx->num != NULL )
    {
        std::string num = ctx->num->getText();
        number = atoi(num.c_str());
    }

    // Match subtype with type.
    std::list< Architecture::DomainSpecificType * >::iterator lit;
    for( lit = _network->types.begin();
            lit != _network->types.end();
            ++lit )
    {
        Architecture::DomainSpecificType * dst = *lit;
        std::string dstname = dst->getName();
        size_t dstlen = dstname.size();
        size_t slen = name.size();
        if( dstlen <= slen )
        {
            std::string sstr = name.substr(slen - dstlen);
            /// \todo Implement groups.

            if( sstr == dstname )
            {
                std::string gstr = name.substr(0,(slen-dstlen) - 1);

                Architecture::ComponentsGroup * gr = new
                    Architecture::ComponentsGroup(dst);

                std::list< std::string > l = Manipulation::_getTokens(gstr);
                std::list< std::string >::iterator iter;
                for( iter = l.begin(); iter != l.end(); ++iter )
                {
                    Architecture::TopologyParameter tp =
                        (Architecture::TopologyParameter) (*iter);
                    gr->parameters.push_back(tp);
                }

                std::list< Architecture::NetworkComponent * > components;

                for( unsigned int i = 1; i <= number; ++i )
                {

                    std::string cname(name);
                    cname += std::string(" ");
                    cname += std::to_string(i);
                    Architecture::NetworkComponent * cmp =
                        new Architecture::NetworkComponent(cname,dst);
                    _network->components.push_back(cmp);
                    components.push_back(cmp);
                }

                if( ! components.empty())
                {
                    std::pair<
                        Architecture::ComponentsGroup *,
                        std::list< Architecture::NetworkComponent * > > p;
                    p.first = gr;
                    p.second = components;
                    _network->groupsmap.insert(p);
                    _network->groups.push_back(gr);

                }
                return;
            }
        }



        else
        {
            continue;
        }
    }
}

void SpecBuilder::enterAbbrev( ChaseParser::AbbrevContext * ctx )
{
    if( ctx->abbr != NULL )
    {
        std::string name = _getNameFromContext( ctx->nam );

        // Search component in list. If it is not found generate an error.

        std::list< Architecture::NetworkComponent * >::iterator cit;
        for( cit = _network->components.begin();
                cit != _network->components.end();
                ++cit )
        {
            Architecture::NetworkComponent * comp = *cit;
            if( comp->getName() == name )
            {
                comp->setAbbreviation(ctx->abbr->getText());
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Network Connections Managing
//
////////////////////////////////////////////////////////////////////////////////

void SpecBuilder::enterConnections(ChaseParser::ConnectionsContext * ctx)
{
    assert(ctx);
    _inConnections = true;
    _inSwitched = false;
    _inUnswitched = false;
}


void SpecBuilder::exitConnections(ChaseParser::ConnectionsContext * ctx)
{
    assert(ctx);
    _inConnections = false;
    _inSwitched = false;
    _inUnswitched = false;
}


void SpecBuilder::enterSwitched(ChaseParser::SwitchedContext * ctx)
{
    assert(ctx);
    _inSwitched = true;
    _inUnswitched = false;
    std::string sw = _getNameFromContext(ctx->sw);
    std::list< Architecture::DomainSpecificType * >::iterator iter;
    for( iter = _network->types.begin();
            iter != _network->types.end(); ++iter )
    {
        Architecture::DomainSpecificType * dswt = *iter;
        if( dswt->getName() == sw 
                && dswt->getGenericType() == Architecture::ct_switch)
        {
            _currSwitchType = 
                static_cast< Architecture::DomainSpecificSwitchType * >(dswt);
        }
    }

}

void SpecBuilder::enterUnswitched(ChaseParser::UnswitchedContext * ctx)
{
    assert(ctx);
    _inUnswitched = true;
    _inSwitched = false;
}



void SpecBuilder::enterConn(ChaseParser::ConnContext * ctx)
{

    if( _inConnections && _inSwitched && !_inUnswitched )
    {
        _enterSwitchedConn(ctx);
    }
    else if( _inConnections && ! _inSwitched && _inUnswitched )
    {
        _enterUnswitchedConn(ctx);

    }
    else if( _inConnections && ! _inSwitched && ! _inUnswitched )
    {
        _enterSpecificConn(ctx);
    }
    else
    {
        _errors.push_back(std::string("Something wrong in connections"));
    }
}   

Architecture::NetworkConnection * SpecBuilder::_createConnection(
        Architecture::NetworkComponent * f,
        Architecture::NetworkComponent * t,
        bool switched )
{


    // Search to see if the connection already exists.
    std::list< Architecture::NetworkConnection * >::iterator it;
    for( it = _network->connections.begin();
            it != _network->connections.end(); ++it )
    {
        Architecture::NetworkConnection * conn = *it;
        if( f == conn->getTarget() && t == conn->getSource() )
        {
            conn->setBidirectional(true);
            return conn;
        }

    }
    Architecture::NetworkConnection * conn; 
    if( switched )
    {
        std::string swname(_currSwitchType->getName());
        swname += std::string(" ");
        swname += _gen_random(5, _network);

        Architecture::NetworkComponent * swcomp = 
            new Architecture::NetworkComponent( swname, _currSwitchType );

        _network->switches.push_back(swcomp);

        conn = new Architecture::NetworkConnection( f, t, swcomp );
    } else {
        conn = new Architecture::NetworkConnection( f, t );
    }

    return conn;

}


void SpecBuilder::_enterSwitchedConn( ChaseParser::ConnContext * ctx )
{    
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);

    Manipulation::FindComponentsOptions opt;
    std::set< Architecture::NetworkComponent * > fromset = 
        Manipulation::findComponents( from, _network, opt );
    std::set< Architecture::NetworkComponent * > toset = 
        Manipulation::findComponents( to, _network, opt );

    std::set< Architecture::NetworkComponent * >::iterator i;
    for( i = fromset.begin(); i != fromset.end(); ++i )
    {
        std::set< Architecture::NetworkComponent * >::iterator j;
        for( j = toset.begin(); j != toset.end(); ++j )
        {
            // To do not create selfconnections.
            if( *j == *i ) continue;

            // Create the connection.
            Architecture::NetworkComponent * f = *i;
            Architecture::NetworkComponent * t = *j;

            Architecture::NetworkConnection * conn = 
                _createConnection(f,t,true);
            if( conn != NULL )
            {
                _network->connections.push_back(conn);
            }

            if( f->getType() == t->getType() )
            {
                conn = 
                    _createConnection(t,f,true);
                if( conn != NULL )
                {
                    _network->connections.push_back(conn);
                }

            }
        }
    }
}

void SpecBuilder::_enterUnswitchedConn( ChaseParser::ConnContext * ctx )
{
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);

    Manipulation::FindComponentsOptions opt;
    std::set< Architecture::NetworkComponent * > fromset = 
        Manipulation::findComponents( from, _network, opt );
    std::set< Architecture::NetworkComponent * > toset = 
        Manipulation::findComponents( to, _network, opt );

    std::set< Architecture::NetworkComponent * >::iterator i;
    for( i = fromset.begin(); i != fromset.end(); ++i )
    {
        std::set< Architecture::NetworkComponent * >::iterator j;
        for( j = toset.begin(); j != toset.end(); ++j )
        {
            // To do not create selfconnections.
            if( *j == *i ) continue;
            Architecture::NetworkComponent * f = *i;
            Architecture::NetworkComponent * t = *j;

            Architecture::NetworkConnection * conn = 
                _createConnection(f,t,false);
            if( conn != NULL )
            {
                _network->connections.push_back(conn);
            }

            if( f->getType() == t->getType() )
            {
                conn =
                    _createConnection(t,f,false);
                if( conn != NULL )
                {
                    _network->connections.push_back(conn);
                }
            }

        }
    }
}

void SpecBuilder::_enterSpecificConn( ChaseParser::ConnContext * ctx )
{
    std::string from = _getNameFromContext(ctx->from);
    std::string to = _getNameFromContext(ctx->to);
    std::string sw = _getNameFromContext(ctx->sw);
    Manipulation::FindComponentsOptions opt;

    std::set< Architecture::NetworkComponent * > fromset = 
        Manipulation::findComponents( from, _network, opt );
    std::set< Architecture::NetworkComponent * > toset = 
        Manipulation::findComponents( to, _network, opt );

    Architecture::DomainSpecificType * dst = NULL;
    std::list< Architecture::DomainSpecificType * >::iterator it;
    for( it = _network->types.begin();
            it != _network->types.end();
            ++it )
    {
        Architecture::DomainSpecificType * t = *it;
        std::string n = t->getName();
        if( sw.find(n) != std::string::npos )
        {
            dst = t;
            break;
        }
    }
    if( dst == NULL )
    {
        std::cerr << "Impossible to identify the type of: "
            << sw << std::endl;
        exit(-1);
    }

    std::set< Architecture::NetworkComponent * >::iterator i;
    for( i = fromset.begin(); i != fromset.end(); ++i )
    {
        std::set< Architecture::NetworkComponent * >::iterator j;
        for( j = toset.begin(); j != toset.end(); ++j )
        {
            // To do not create selfconnections.
            if( *j == *i ) continue;
            Architecture::NetworkComponent * f = *i;
            Architecture::NetworkComponent * t = *j;
            Architecture::NetworkComponent * swtch = 
                new Architecture::NetworkComponent( sw, dst );
            _network->switches.push_back(swtch);

            Architecture::NetworkConnection * conn =
                new Architecture::NetworkConnection(f,t,swtch);
            _network->connections.push_back(conn);

            if( f->getType() == t->getType() )
            {
                conn =
                    _createConnection(t,f,false);
                if( conn != NULL )
                {
                    _network->connections.push_back(conn);
                }
            }

        }
    }
}



///////////////////////////////////////////////////////////////////////////////
// Read requirements and assumptions
///////////////////////////////////////////////////////////////////////////////

void SpecBuilder::enterRequirements(ChaseParser::RequirementsContext * ctx)
{
    assert(ctx);
    _inRequirements = true;
    _inAssumptions = false;
}

void SpecBuilder::enterAssumptions(ChaseParser::AssumptionsContext * ctx)
{
    assert(ctx);
    _inRequirements = false;
    _inAssumptions = true;
}

void SpecBuilder::exitRequirements(ChaseParser::RequirementsContext * ctx)
{
    assert(ctx);
    _inRequirements = false;
}

void SpecBuilder::exitAssumptions(ChaseParser::AssumptionsContext * ctx)
{
    assert(ctx);
    _inAssumptions = false;
}



void SpecBuilder::enterReq(ChaseParser::ReqContext * ctx)
{
    Specification::Requirement * req = new Specification::Requirement();
    if(ctx->pred != nullptr)
    {
        req->setFunctionName(ctx->pred->getText());
    }
    _currRequirement = req;
    if( _inRequirements )
        _problem->requirements.push_back(req);
    if( _inAssumptions )
        _problem->assumptions.push_back(req);

}

void SpecBuilder::enterParam(ChaseParser::ParamContext * ctx)
{
    if(ctx->num != nullptr)
    {
        _currRequirement->_params.push_back(ctx->num->getText());
    }
    if( ctx->nam != nullptr )
    {
        std::string par = _getNameFromContext(ctx->nam);
        _currRequirement->_params.push_back(par);
    }

}


