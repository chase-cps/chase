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
 * @file	chase/src/Architecture/EPS/EPSTopology.cc
 * @brief
 */



#include "EPS/EPSTopology.hh"
#include <sstream>

using namespace chase::epstool;
using namespace EPS;
using namespace Architecture;
using namespace Contracts;
using namespace Behaviors;

using namespace Logics;

using namespace Utilities::Portability;

EPSTopology::EPSTopology( std::string spec, bool phys ) :
    components(),
    contactors(),
    _adj_matrix(NULL),
    _physical_contactors(phys)
{
    _parseFile( spec );
    for( int i = 0; i < COMPONENT_TYPE_SIZE; ++i )
    {
        _n_per_type[i] = 0u;
    } 
}

EPSTopology::~EPSTopology()
{
}

void EPSTopology::_parseFile( std::string spec )
{
    // Open the input file
    std::ifstream fileIn;
    fileIn.open(spec.c_str(), std::ifstream::in);
    if( ! fileIn.good() )
    {
        std::cerr << "File " << spec << " does not exist" << std::endl;
        exit(-1);
    }

    // Split File
    std::list<std::string> * scomponents = new std::list< std::string >();
    std::list<std::string> * smatrix = new std::list< std::string >();
    std::list<std::string> * sdirect_connections = new std::list< std::string >();

    bool inComponents = false;
    bool inMatrix = false;
    bool inDirectConnections = false;

    std::string currentLine;
    safeGetline(fileIn, currentLine);
    while(! fileIn.eof() )
    {
        if( currentLine == "[COMPONENTS]" )
        {
            inComponents = true;
            inMatrix = false;
            inDirectConnections = false;
        }
        else if ( currentLine == "[MATRIX]" )
        {
            inComponents = false;
            inMatrix = true;
            inDirectConnections = false;
        }
        else if ( currentLine == "[DIRECT CONNECTIONS]" )
        {
            inComponents = false;
            inMatrix = false;
            inDirectConnections = true;
        }
        else
        {
            if( currentLine != "" )
            {
                if( inComponents )
                {
                    scomponents->push_back(currentLine);
                }
                if( inMatrix )
                {
                    smatrix->push_back(currentLine);
                }
                if( inDirectConnections )
                {
                    sdirect_connections->push_back(currentLine);
                }
            }
        }
        safeGetline(fileIn, currentLine);
    }

    // Create Nodes    
    _n_components = _createComponents( scomponents ); 

    // Create Matrix
    _adj_matrix = new int*[_n_components];
    for( int i = 0; i < _n_components; ++i )
    {
        _adj_matrix[i] = new int[_n_components];
    }
    for( int i = 0; i < _n_components; ++i )
    {
        for( int j = 0; j < _n_components; ++j )
        {
            _adj_matrix[i][j] = 0;
        }
    }

    _createMatrix( smatrix );

    _createContactors( sdirect_connections );

    // Close the input file
    fileIn.close();

    delete(scomponents);
    delete(smatrix);
    delete(sdirect_connections);

}

int EPSTopology::_createComponents( std::list< std::string > * components )
{
    int n_comp = 0;

    std::list< std::string >::iterator line_it;
    for( line_it = components->begin();
            line_it != components->end();
            ++line_it )
    {
        std::string declaration = *line_it;
        size_t colon_position = declaration.find(':');

        std::string component = declaration.substr(0, colon_position-1);
        std::string number = declaration.substr(colon_position+1);

        component.erase(
                std::remove(component.begin(),component.end(),' '),
                component.end());
        number.erase(
                std::remove(number.begin(),number.end(),' '),
                number.end());

        component.erase(
                std::remove(component.begin(),component.end(),'\t'),
                component.end());
        number.erase(
                std::remove(number.begin(),number.end(),'\t'),
                number.end());

        component.erase(
                std::remove(component.begin(),component.end(),'\n'),
                component.end());
        number.erase(
                std::remove(number.begin(),number.end(),'\n'),
                number.end());

        n_comp += _declareComponent(component, atoi(number.c_str()));


    }

    return n_comp;
}

int EPSTopology::_createMatrix( std::list< std::string > * matrix )
{
    int edges = 0;
    int line_number = 0;
    std::list< std::string >::iterator line_it;
    for( line_it = matrix->begin();
            line_it != matrix->end();
            ++line_it )
    {
        std::string line = *line_it;
        line.erase(
                std::remove(line.begin(), line.end(),' '),
                line.end());
        line.erase(
                std::remove(line.begin(), line.end(),'\n'),
                line.end());
        line.erase(
                std::remove(line.begin(), line.end(),'\t'),
                line.end());

        for( size_t i = 0; i < line.length(); ++i )
        {
            if( line[i] == '1' )
            {
                _adj_matrix[line_number][i] = 1;
                ++edges;
            }
            else if( line[i] == '0' )
            {
                _adj_matrix[line_number][i] = 0;
            }
        }
        ++line_number;
    }



    return edges;
}


int EPSTopology::_declareComponent( std::string c, unsigned int n )
{
    int number = n;
    componentType type;

    std::map< std::string, typeStruct >::iterator m_it = typeMap.find(c);

    if( m_it == typeMap.end() )
    {
        std::cerr << "Wrong declaration of type: " << c << std::endl;
        std::cerr << "abort the execution" << std::endl;
        exit(-1);
    }

    typePair p = *m_it;
    typeStruct s = p.second;
    type = *(s.subtypes.begin());

    // The strange stuffs down here are used to allow to declare in 
    // more than one time all the components belonging to a type.
    //
    // e.g., To declare three left_generators, it can be done as follows:
    // left_generators : 1
    // ...
    // left_generators: 2
    unsigned int prev_n_per_type = _n_per_type[type];
    _n_per_type[type] += n;

    for( unsigned int i = prev_n_per_type; i < _n_per_type[type]; ++i )
    {
        EPSComponent * comp = new EPSComponent();
        std::string index = std::to_string(i);
    
        std::string name = s.name_radix;
        name += "_";
        name += index;

        comp->name = name;
        comp->type = type;
        components.push_back(comp);
    }
    return number;
}

int EPSTopology::_createContactors( 
        std::list< std::string > * direct_connections )
{
    int inserted_contactors = 0;

    std::multimap< componentType, componentType > * _map = 
        _createDirectConnectionsMap( direct_connections );

    for( int i = 0; i < _n_components; ++i )
    {
        for( int j = 0; j < _n_components; ++j )
        {
            if( _adj_matrix[i][j] == 1 )
            {
                EPSComponent * source = components[i];
                EPSComponent * target = components[j];
                bool direct = false;
                std::multimap< componentType, componentType >::iterator mit;
                for( mit = _map->begin(); mit != _map->end(); ++mit )
                {
                    if( ((*mit).first == source->type) &&
                            ((*mit).second == target->type ))
                    {
                        direct = true;
                        break;
                    }
                }
                if( direct == false )
                {
                    int bidirectional = _checkBidirectionalContactors(i, j);
                    if( bidirectional != -1 )
                    {
                        _adj_matrix[i][j] = bidirectional + 2;
                    } 
                    else 
                    {
                        ++inserted_contactors;
                        _adj_matrix[i][j] = 1 + inserted_contactors;

                        Contactor * contactor; 
                        if( ! _physical_contactors )
                        {
                            contactor = new Contactor();
                        }
                        else
                        {
                            contactor = new PhysicalContactor();
                        }
                        contactor->node_1 = source;
                        contactor->node_2 = target;
                        contactor->number = _adj_matrix[i][j] - 2;
                        contactors.push_back(contactor);
                    }
                }
            }
        }
    }

    return inserted_contactors;
}

    std::multimap< componentType, componentType > *
EPSTopology::_createDirectConnectionsMap( std::list< std::string > * s )
{
    std::multimap< componentType, componentType > * _map = 
        new std::multimap< componentType, componentType >();

    std::list< std::string >::iterator lit;
    for( lit = s->begin(); lit != s->end(); ++lit )
    {
        std::string current = *lit;
        size_t colon_position = current.find("->");

        std::string ssource = current.substr(0, colon_position-1);
        std::string starget = current.substr(colon_position+2);

        // Clean with spaces:
        ssource = Utilities::cleanString(ssource);
        starget = Utilities::cleanString(starget); 
        //

        componentType tsource;
        componentType ttarget;

        std::map< std::string, typeStruct >::iterator m_it;
        m_it = typeMap.find(ssource);

        if( m_it == typeMap.end() )
        {
            std::cerr << "Wrong type in hard connections: \"" 
                << ssource << "\"" << std::endl;
            std::cerr << "abort the execution" << std::endl;
            exit(-1);
        }

        tsource = *((*m_it).second.subtypes.begin());

        m_it = typeMap.find(starget);

        if( m_it == typeMap.end() )
        {
            std::cerr << "Wrong type in hard connections: \"" 
                << ssource << "\"" << std::endl;
            std::cerr << "abort the execution" << std::endl;
            exit(-1);
        }

        ttarget = *((*m_it).second.subtypes.begin());


        std::pair< componentType, componentType > p(tsource, ttarget);
        _map->insert(p);

    }

    return _map;

}

int EPSTopology::_checkBidirectionalContactors(int i, int j)
{
    int existing = -1;
    std::vector< Contactor * >::iterator it; 
    for( it = contactors.begin(); it != contactors.end(); ++it )
    {
        Contactor * curr = *it;
        if( (curr->node_1 == components[i] && curr->node_2 == components[j]) ||
                (curr->node_2 == components[i] && curr->node_1 == components[j]))
        {
            existing = curr->number;
            return existing;
        }
    }
    return existing;
}

int ** EPSTopology::getMatrix()
{
    return _adj_matrix;
}

int EPSTopology::getSize()
{
    return _n_components;
}

Contracts::AGContract * EPSTopology::buildContractFromTopology()
{
    AGContract * contract = new AGContract();

    int num_components = components.size();
    int num_contactors = contactors.size();

    for( int i = 0; i < num_components; ++i )
    {
        // Put components state variables into the environment variables of 
        // the contract.
        std::string name("state_");
        name += components[i]->name;

        LogicProposition * var = new LogicProposition(name);

        components[i]->stateVariable = var;

        contract->environment_variables.push_back(var);

    }

    for( int i = 0; i < num_contactors; ++i )
    {
        // Put the contactors state variables into the environment variables of 
        // the contract.
        std::string name_state("state_contactor_");
        std::stringstream ss;
        ss << contactors[i]->number;
        name_state += std::string(ss.str());

        LogicProposition * state_var = new LogicProposition(name_state);
        contactors[i]->stateVariable = state_var;
        if( _physical_contactors )
        {
            contract->environment_variables.push_back(state_var);
        }
        else
        {
            contract->system_variables.push_back(state_var);
        }

        if( _physical_contactors )
        {
            PhysicalContactor * contactor = static_cast< PhysicalContactor * >(
                    contactors[i] );
            // Put the contactors intention variables into the system variables
            // of the contract.
            std::string name_intention("intention_contactor_");
            ss.str(std::string());
            ss << contactor->number;
            name_intention += std::string(ss.str());

            LogicProposition * intention_var = new LogicProposition(name_intention);
            contactor->intentionVariable = intention_var;
            contract->system_variables.push_back(intention_var);

            // Put the contactors counter variables into the environment variables
            // of the contract.
            //
            // Opening
            //
            std::string name_opening("opening_counter_contactor_");
            ss.str(std::string());
            ss << contactor->number;
            name_opening += std::string(ss.str());


            // Closing 
            // 
            std::string name_closing("closing_counter_contactor_");
            ss.str(std::string());
            ss << contactor->number;
            name_closing += std::string(ss.str());

        }

    }

    return contract;


}


Contactor * EPSTopology::getContactorBetweenNodes(int i, int j)
{
    int index = _adj_matrix[i][j] - 2;
    if( (contactors[index]->node_1 == components[i] 
                && contactors[index]->node_2 == components[j]) ||
            (contactors[index]->node_1 == components[j] 
             && contactors[index]->node_2 == components[i] )
      )
    {
        return contactors[index];
    } else {
        return NULL;
    }
}

int ** EPSTopology::getInverseMatrix()
{
    int ** newMatrix = new int*[_n_components];
    for( int i = 0; i < _n_components; ++i )
    {
        newMatrix[i] = new int[_n_components];
        for( int j = 0; j < _n_components; ++j )
        {
            newMatrix[i][j] = _adj_matrix[j][i];
        }
    }
    return newMatrix;
}



