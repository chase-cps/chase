/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/27/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"
using namespace chase;

void LogisticsSpecsBuilder::_createContracts() const {
    for(auto road : warehouse->roads)
    {
        auto c = _createRoadContract(road);
        if(c != nullptr) warehouse->component2Contract.insert(
                std::pair< Equipment *, Contract * >(
                        reinterpret_cast<Equipment*>(road), c));
    }
}


Contract *LogisticsSpecsBuilder::_createRoadContract(Road *road) const
{
    auto c = new Contract(road->name);
    std::vector< Variable * > Fin;
    std::vector< Variable * > Fout;

    auto v = new Variable(new Real(),
                          new Name(road->name+ "_F0_in"),
                          chase::input);
    Fin.push_back(v);
    c->addDeclaration(v);
    v = new Variable(new Real(),
                     new Name(road->name+ "_F0_out"),
                     chase::output);
    Fout.push_back(v);
    c->addDeclaration(v);

    for(size_t i = 0; i < warehouse->products.size(); ++i)
    {
        unsigned int flow = i + 1;
        v = new Variable(new Real(), new Name(
                road->name+ "_F" + std::to_string(flow) + "_in"),
                         chase::input);
        Fin.push_back(v);
        c->addDeclaration(v);
        v = new Variable(new Real(),new Name(
                road->name+ "_F" + std::to_string(flow) + "_out"),
            chase::output);
        Fout.push_back(v);
        c->addDeclaration(v);

        /// \todo For each variable, set the bound.

    }
    return c;
}

Contract *LogisticsSpecsBuilder::_createCrossroadContract(
        Crossroad *xroad) const {
    return nullptr;
}

Contract *LogisticsSpecsBuilder::_createForumContract(
        Forum *forum) const {
    return nullptr;
}

Contract *LogisticsSpecsBuilder::_createPickingStationContract(
        PickingStation *station) const {
    return nullptr;
}

