/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/16/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Distribution.hh"
#include "chase/representation/Value.hh"

using namespace chase;

Distribution::Distribution(Type * type, Name * name) :
        DataDeclaration(type, name)
{
    _node_type = distribution_node;
}

Distribution::~Distribution() {
}

distribution_type Distribution::getDistributionType() const {
    return _distribution_type;
}

void Distribution::setDistributionType(distribution_type distributionType) {
    _distribution_type = distributionType;
}

int Distribution::accept_visitor(BaseVisitor &v) {
    return v.visitDistribution(*this);
}

std::string Distribution::getString()
{
    std::string ret("distribution ");
    ret += _name->getString() + std::string(" is \n");
    ret += "\t" + _type->getString() + ",\n";
    ret += "\t";
    // The print is based on the type of distribution.
    switch(_distribution_type) {
        case custom:
            ret += "\tcustom (\n";
            break;
        case gaussian:
            ret += "\tgaussian (\n";
            break;
        case homogeneous:
            ret += "\tgaussian (\n";
            break;
        default:
            messageError("Not recognized type of distribution");
            break;
    }
    for(auto it: parameters)
        ret += "\t" + it.first + " = " + it.second->getString();
    ret += "\t)\n";
    return ret;
}

Distribution * Distribution::clone() {
    auto distribution = new Distribution(
            _type->clone(), _name->clone());
    distribution->setDistributionType(_distribution_type);
    for(auto it :   parameters)
        distribution->parameters.insert(
                std::pair<std::string, chase::Value*>(
                        it.first, it.second->clone()));
    return distribution;
}

