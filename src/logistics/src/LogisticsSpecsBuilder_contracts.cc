/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/27/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"
#include "utilities/Factory.hh"
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

    auto * assumptions = new LargeBooleanFormula(op_and);
    auto * guarantees = new LargeBooleanFormula(op_and);

    auto vin = new Variable(new Real(),
                          new Name(road->name+ "_F0_in"),
                          chase::input);
    Fin.push_back(vin);
    c->addDeclaration(vin);

    assumptions->addOperand(
            Prop(GE(Id(vin), RealVal(0.0))));
    assumptions->addOperand(
            Prop(LE(Id(vin), RealVal(1.0))));

    auto vout = new Variable(new Real(),
                     new Name(road->name+ "_F0_out"),
                     chase::output);
    Fout.push_back(vout);
    c->addDeclaration(vout);

    guarantees->addOperand(
            Prop(GE(Id(vout), RealVal(0.0))));
    guarantees->addOperand(
            Prop(LE(Id(vout), RealVal(1.0))));

    for(size_t i = 0; i < warehouse->products.size(); ++i)
    {
        unsigned int flow = i + 1;
        vin = new Variable(new Real(), new Name(
                road->name+ "_F" + std::to_string(flow) + "_in"),
                         chase::input);
        Fin.push_back(vin);
        c->addDeclaration(vin);

        assumptions->addOperand(
                Prop(GE(Id(vin), RealVal(0.0))));
        assumptions->addOperand(
                Prop(LE(Id(vin), RealVal(1.0))));

        vout = new Variable(new Real(),new Name(
                road->name+ "_F" + std::to_string(flow) + "_out"),
            chase::output);
        Fout.push_back(vout);
        c->addDeclaration(vout);

        guarantees->addOperand(
                Prop(GE(Id(vout), RealVal(0.0))));
        guarantees->addOperand(
                Prop(LE(Id(vout), RealVal(1.0))));

        guarantees->addOperand(
                Prop(Eq(Id(vout), Id(vin))));
    }

    auto sum_in = reinterpret_cast<Value*>(Id(Fin[0]));
    auto sum_out = reinterpret_cast<Value*>(Id(Fout[0]));

    for(size_t i = 1; i < Fin.size(); ++i) {
        sum_in = reinterpret_cast<Value*>(Sum(sum_in, Id(Fin[i])));
    }

    for(size_t i = 1; i < Fout.size(); ++i) {
        sum_out = reinterpret_cast<Value*>(Sum(sum_out, Id(Fout[i])));
    }

    assumptions->addOperand(Prop(LE(sum_in, RealVal(1.0))));
    assumptions->addOperand(Prop(GE(sum_in, RealVal(0.0))));

    guarantees->addOperand(Prop(LE(sum_out, RealVal(1.0))));
    guarantees->addOperand(Prop(GE(sum_out, RealVal(0.0))));

    c->addAssumptions(chase::constraints, assumptions);
    c->addGuarantees(chase::constraints, guarantees);
    std::cout << c->getString() << std::endl;
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

