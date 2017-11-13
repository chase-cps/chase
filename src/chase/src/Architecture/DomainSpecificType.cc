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
 * @file	chase/src/Architecture/DomainSpecificType.cc
 * @brief
 */



#include "Architecture/DomainSpecificType.hh"

using namespace Architecture;

DomainSpecificType::DomainSpecificType(
        std::string name,
        component_types type ) :
    _name(name),
    _generic_type(type),
    _indegree_range(NULL),
    _outdegree_range(NULL),
    _flow_params(0),
    _flow_parameters(NULL)
{
    unsigned int im = 0;
    unsigned int iM = UINT_MAX;

    unsigned int om = 0;
    unsigned int oM = UINT_MAX;

    switch(type)
    {
        case ct_generic:
            break;
        case ct_source:
            iM = 0;
            om = 1;
            oM = 1;
            _flow_params = 1;
            break;
        case ct_bus:
            _flow_params = 1;
            im = 1;
            om = 1;
            break;
        case ct_converter:
            im = 1;
            iM = 1;
            om = 1;
            oM = 1;
            _flow_params = 2;
            break;
        case ct_sink:
            im = 1;
            iM = 1;
            oM = 0;
            _flow_params = 1;
            break;
        case ct_switch:
            im = 1;
            iM = 1;
            om = 1;
            oM = 1;
            break;
        default:
            break;
    }
    _indegree_range = new degree_range(im,iM);
    _outdegree_range = new degree_range(om,oM);

    if( _flow_params > 0 )
    {
        _flow_parameters = new FlowParameter[_flow_params];
    }
}

DomainSpecificType::~DomainSpecificType()
{
    delete _indegree_range;
    delete _outdegree_range;
}

degree_range * DomainSpecificType::getInDegreeRange()
{
    return _indegree_range;
}

degree_range * DomainSpecificType::getOutDegreeRange()
{
    return _outdegree_range;
}

void DomainSpecificType::setGenericType( 
        component_types type,
        unsigned int indegree_min,
        unsigned int indegree_max,
        unsigned int outdegree_min,
        unsigned int outdegree_max )
{
    _generic_type = type;
    unsigned int im, iM, om, oM;
    switch(type)
    {
        case ct_generic:
            im = indegree_min;
            iM = indegree_max;
            om = outdegree_min;
            oM = outdegree_max;
            break;
        case ct_source:
            im = 0;
            iM = 0;
            om = 1;
            oM = 1;
            if( indegree_min != 0 || 
                    indegree_max != 0 ||
                    outdegree_min != 1 ||
                    outdegree_max != 1 )
            {
                std::cerr << "WARNING:" << std::endl;
                std::cerr << "Trying to assign a illegal range to generic type"
                    << std::endl;
            }
            break;
        case ct_bus:
            im = indegree_min;
            iM = indegree_max;
            om = outdegree_min;
            oM = outdegree_max;
            break;
        case ct_converter:
            im = 1;
            iM = 1;
            om = 1;
            oM = 1;
            if( indegree_min != 1 || 
                    indegree_max != 1 ||
                    outdegree_min != 1 ||
                    outdegree_max != 1 )
            {
                std::cerr << "WARNING:" << std::endl;
                std::cerr << "Trying to assign a illegal range to generic type"
                    << std::endl;
            }
            break;
        case ct_sink:
            im = 1;
            iM = 1;
            oM = 0;
            om = 0;
            if( indegree_min != 1 || 
                    indegree_max != 1 ||
                    outdegree_min != 0 ||
                    outdegree_max != 0 )
            {
                std::cerr << "WARNING:" << std::endl;
                std::cerr << "Trying to assign a illegal range to generic type"
                    << std::endl;
            }
            break;
        case ct_switch:
            im = 1;
            iM = 1;
            om = 1;
            oM = 1;
            if( indegree_min != 1 || 
                    indegree_max != 1 ||
                    outdegree_min != 1 ||
                    outdegree_max != 1 )
            {
                std::cerr << "WARNING:" << std::endl;
                std::cerr << "Trying to assign a illegal range to generic type"
                    << std::endl;
            }
            break;
        default:
            im = 0;
            iM = UINT_MAX;   
            om = 0;
            oM = UINT_MAX;
            break;
    }
    _indegree_range->min = im;
    _indegree_range->max = iM;
    _outdegree_range->min = om;
    _outdegree_range->max = oM;
}

component_types DomainSpecificType::getGenericType()
{
    return _generic_type;
}


std::string DomainSpecificType::getName()
{
    return _name;
}

bool DomainSpecificType::addParameter(unsigned int position, FlowParameter param)
{
    if( _flow_parameters != NULL && position < _flow_params )
    {
        _flow_parameters[position] = param;
        return true;
    }
    return false;
}

