#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-inefficient-string-concatenation"
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
    for(auto road : warehouse->roads) {
        auto c = _createRoadContract(road);
        if(c != nullptr) warehouse->component2Contract.insert(
                std::pair< Equipment *, Contract * >(
                        reinterpret_cast<Equipment*>(road), c));
    }
    for(auto xroad : warehouse->crossroads) {
        auto c = _createCrossroadContract(xroad);
        if(c != nullptr) warehouse->component2Contract.insert(
                    std::pair< Equipment *, Contract * >(
                            reinterpret_cast<Equipment*>(xroad), c));
    }
    for(auto forum : warehouse->forums) {
        auto c = _createForumContract(forum);
        if(c != nullptr) warehouse->component2Contract.insert(
                    std::pair< Equipment *, Contract * >(
                            reinterpret_cast<Equipment*>(forum), c));
    }
    for(auto station : warehouse->stations) {
        auto c = _createPickingStationContract(station);
        if(c != nullptr) warehouse->component2Contract.insert(
                    std::pair< Equipment *, Contract * >(
                            reinterpret_cast<Equipment*>(station), c));
    }

}


Contract *LogisticsSpecsBuilder::_createRoadContract(Road *road) const
{

    auto c = new Contract(road->name);
    std::vector< Variable * > Fin;
    std::vector< Variable * > Fout;

    auto * assumptions = new LargeBooleanFormula(op_and);
    auto * guarantees = new LargeBooleanFormula(op_and);

    Value * zero_inflow_sum = nullptr;
    Value * zero_outflow_sum = nullptr;

    for(auto entry : road->entries)
    {
        std::string exit_name(road->name);
        std::string entry_name(entry->name);
        std::string name(entry_name + "_to_" + exit_name + "_0");

        auto vin = new Variable(new Real(),
                                new Name(name),
                                chase::generic);

        Fin.push_back(vin);
        c->addDeclaration(vin);

        if(zero_inflow_sum == nullptr) zero_inflow_sum = Id(vin);
        else zero_inflow_sum = Sum(zero_inflow_sum, Id(vin));

        assumptions->addOperand(
                Prop(GE(Id(vin), RealVal(0.0))));
        assumptions->addOperand(
                Prop(LE(Id(vin), RealVal(1.0))));
    }

    for(auto exit: road->exits)
    {
        std::string name(road->name + "_to_" + exit->name + "_0");

        auto vout = new Variable(new Real(),
                                 new Name(name),
                                 chase::generic);
        Fout.push_back(vout);
        c->addDeclaration(vout);

        if(zero_outflow_sum == nullptr) zero_outflow_sum = Id(vout);
        else zero_outflow_sum = Sum(zero_outflow_sum, Id(vout));

        guarantees->addOperand(
                Prop(GE(Id(vout), RealVal(0.0))));
        guarantees->addOperand(
                Prop(LE(Id(vout), RealVal(1.0))));
    }

    for(size_t i = 0; i < warehouse->products.size(); ++i)
    {
        unsigned int flow = i + 1;

        Value * inflow = nullptr;
        Value * outflow = nullptr;

        for(auto entry : road->entries) {
            std::string entry_name(entry->name);
            std::string exit_name(road->name);

            std::string name(entry_name + "_to_"
                + exit_name + "_" + std::to_string(flow));

            auto cvin = new Variable(new Real(), new Name(name),
                                     chase::generic);

            Fin.push_back(cvin);
            c->addDeclaration(cvin);

            if(inflow == nullptr) inflow = Id(cvin);
            else inflow = Sum(inflow, Id(cvin));

            assumptions->addOperand(
                    Prop(GE(Id(cvin), RealVal(0.0))));
            assumptions->addOperand(
                    Prop(LE(Id(cvin), RealVal(1.0))));
        }

        for(auto exit : road->exits)
        {
            std::string name(road->name + "_to_" + exit->name + "_" + std::to_string(flow));

            auto cvout = new Variable(new Real(), new Name(name),
                                      chase::generic);
            Fout.push_back(cvout);
            c->addDeclaration(cvout);

            if(outflow == nullptr) outflow = Id(cvout);
            else outflow = Sum(outflow, Id(cvout));

            guarantees->addOperand(
                    Prop(GE(Id(cvout), RealVal(0.0))));
            guarantees->addOperand(
                    Prop(LE(Id(cvout), RealVal(1.0))));
        }

        auto p = warehouse->products[i];


        auto found = road->quantities.find(p);
        if(found == road->quantities.end() || found->second == 0) {
            guarantees->addOperand(
                    Prop(Eq(outflow, inflow)));
        }
        else
        {
            guarantees->addOperand(
                    Prop(GE(zero_inflow_sum, zero_outflow_sum)));
            guarantees->addOperand(
                    Prop(GE(outflow, inflow)));
        }
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
    guarantees->addOperand(Prop(Eq(sum_in, sum_out)));

    c->addAssumptions(chase::constraints, assumptions);
    c->addGuarantees(chase::constraints, guarantees);

    if(_params->verbose) std::cout << c->getString() <<std::endl;

    return c;
}

Contract *LogisticsSpecsBuilder::_createCrossroadContract(
        Crossroad *xroad) const {
    auto c = new Contract(xroad->getName());

    std::vector< Variable * > Fin;
    std::vector< Variable * > Fout;

    auto * assumptions = new LargeBooleanFormula(op_and);
    auto * guarantees = new LargeBooleanFormula(op_and);

    Value * zero_inflow_sum = nullptr;
    Value * zero_outflow_sum = nullptr;

    for(auto entry : xroad->entries)
    {
        std::string exit_name(xroad->name);
        std::string entry_name(entry->name);
        std::string name(entry_name + "_to_" + exit_name + "_0");

        auto vin = new Variable(new Real(),
                                new Name(name),
                                chase::generic);

        Fin.push_back(vin);
        c->addDeclaration(vin);

        if(zero_inflow_sum == nullptr) zero_inflow_sum = Id(vin);
        else zero_inflow_sum = Sum(zero_inflow_sum, Id(vin));

        assumptions->addOperand(
                Prop(GE(Id(vin), RealVal(0.0))));
        assumptions->addOperand(
                Prop(LE(Id(vin), RealVal(1.0))));
    }

    for(auto exit: xroad->exits)
    {
        std::string name(xroad->name + "_to_" + exit->name + "_0");

        auto vout = new Variable(new Real(),
                                 new Name(name),
                                 chase::generic);
        Fout.push_back(vout);
        c->addDeclaration(vout);

        if(zero_outflow_sum == nullptr) zero_outflow_sum = Id(vout);
        else zero_outflow_sum = Sum(zero_outflow_sum, Id(vout));

        guarantees->addOperand(
                Prop(GE(Id(vout), RealVal(0.0))));
        guarantees->addOperand(
                Prop(LE(Id(vout), RealVal(1.0))));
    }

    for(size_t i = 0; i < warehouse->products.size(); ++i) {
        unsigned int flow = i + 1;

        Value *inflow = nullptr;
        Value *outflow = nullptr;

        for (auto entry: xroad->entries) {
            std::string entry_name(entry->name);
            std::string exit_name(xroad->name);

            std::string name(entry_name + "_to_"
                             + exit_name + "_" + std::to_string(flow));

            auto cvin = new Variable(new Real(), new Name(name),
                                     chase::generic);

            Fin.push_back(cvin);
            c->addDeclaration(cvin);

            if (inflow == nullptr) inflow = Id(cvin);
            else inflow = Sum(inflow, Id(cvin));

            assumptions->addOperand(
                    Prop(GE(Id(cvin), RealVal(0.0))));
            assumptions->addOperand(
                    Prop(LE(Id(cvin), RealVal(1.0))));
        }

        for (auto exit: xroad->exits) {
            std::string name(xroad->name + "_to_"
                + exit->name + "_" + std::to_string(flow));

            auto cvout = new Variable(new Real(), new Name(name),
                                      chase::generic);
            Fout.push_back(cvout);
            c->addDeclaration(cvout);

            if (outflow == nullptr) outflow = Id(cvout);
            else outflow = Sum(outflow, Id(cvout));

            guarantees->addOperand(
                    Prop(GE(Id(cvout), RealVal(0.0))));
            guarantees->addOperand(
                    Prop(LE(Id(cvout), RealVal(1.0))));
        }
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
    guarantees->addOperand(Prop(Eq(sum_in, sum_out)));

    c->addAssumptions(chase::constraints, assumptions);
    c->addGuarantees(chase::constraints, guarantees);

    if(_params->verbose) std::cout << c->getString() <<std::endl;

    return c;
}

Contract *LogisticsSpecsBuilder::_createForumContract(
        Forum *forum) const {
    auto c = new Contract(forum->getName());

    std::vector< Variable * > Fin;
    std::vector< Variable * > Fout;

    auto * assumptions = new LargeBooleanFormula(op_and);
    auto * guarantees = new LargeBooleanFormula(op_and);

    Value * zero_inflow_sum = nullptr;
    Value * zero_outflow_sum = nullptr;

    for(auto entry : forum->entries)
    {
        std::string exit_name(forum->name);
        std::string entry_name(entry->name);
        std::string name(entry_name + "_to_" + exit_name + "_0");

        auto vin = new Variable(new Real(),
                                new Name(name),
                                chase::generic);

        Fin.push_back(vin);
        c->addDeclaration(vin);

        if(zero_inflow_sum == nullptr) zero_inflow_sum = Id(vin);
        else zero_inflow_sum = Sum(zero_inflow_sum, Id(vin));

        assumptions->addOperand(
                Prop(GE(Id(vin), RealVal(0.0))));
        assumptions->addOperand(
                Prop(LE(Id(vin), RealVal(1.0))));
    }

    for(auto exit: forum->exits)
    {
        std::string name(forum->name + "_to_" + exit->name + "_0");

        auto vout = new Variable(new Real(),
                                 new Name(name),
                                 chase::generic);

        Fout.push_back(vout);
        c->addDeclaration(vout);

        if(zero_outflow_sum == nullptr) zero_outflow_sum = Id(vout);
        else zero_outflow_sum = Sum(zero_outflow_sum, Id(vout));

        guarantees->addOperand(
                Prop(GE(Id(vout), RealVal(0.0))));
        guarantees->addOperand(
                Prop(LE(Id(vout), RealVal(1.0))));
    }

    for(size_t i = 0; i < warehouse->products.size(); ++i) {
        unsigned int flow = i + 1;

        Value *inflow = nullptr;
        Value *outflow = nullptr;

        for (auto entry: forum->entries) {
            std::string entry_name(entry->name);
            std::string exit_name(forum->name);

            std::string name(entry_name + "_to_"
                             + exit_name + "_" + std::to_string(flow));

            auto cvin = new Variable(new Real(), new Name(name),
                                     chase::generic);

            Fin.push_back(cvin);
            c->addDeclaration(cvin);

            if (inflow == nullptr) inflow = Id(cvin);
            else inflow = Sum(inflow, Id(cvin));

            assumptions->addOperand(
                    Prop(GE(Id(cvin), RealVal(0.0))));
            assumptions->addOperand(
                    Prop(LE(Id(cvin), RealVal(1.0))));
        }

        for (auto exit: forum->exits) {
            std::string name(forum->name + "_to_" + exit->name
                    + "_" + std::to_string(flow));

            auto cvout = new Variable(new Real(), new Name(name),
                                      chase::generic);
            Fout.push_back(cvout);
            c->addDeclaration(cvout);

            if (outflow == nullptr) outflow = Id(cvout);
            else outflow = Sum(outflow, Id(cvout));

            guarantees->addOperand(
                    Prop(GE(Id(cvout), RealVal(0.0))));
            guarantees->addOperand(
                    Prop(LE(Id(cvout), RealVal(1.0))));
        }
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
    guarantees->addOperand(Prop(Eq(sum_in, sum_out)));

    c->addAssumptions(chase::constraints, assumptions);
    c->addGuarantees(chase::constraints, guarantees);

    if(_params->verbose) std::cout << c->getString() <<std::endl;

    return c;
}

Contract *LogisticsSpecsBuilder::_createPickingStationContract(
        PickingStation *station) const {
    auto c = new Contract(station->getName());

    std::vector< Variable * > Fin;
    std::vector< Variable * > Fout;
    std::vector< Variable * > Sinked;

    auto assumptions = new LargeBooleanFormula(op_and);
    auto guarantees = new LargeBooleanFormula(op_and);

    Value * zero_inflow_sum = nullptr;
    Value * zero_outflow_sum = nullptr;

    for(auto entry : station->entries)
    {
        std::string exit_name(station->name);
        std::string entry_name(entry->name);
        std::string name(entry_name + "_to_" + exit_name + "_0");

        auto vin = new Variable(new Real(),
                                new Name(name),
                                chase::generic);

        Fin.push_back(vin);
        c->addDeclaration(vin);

        if(zero_inflow_sum == nullptr) zero_inflow_sum = Id(vin);
        else zero_inflow_sum = Sum(zero_inflow_sum, Id(vin));

        assumptions->addOperand(
                Prop(GE(Id(vin), RealVal(0.0))));
        assumptions->addOperand(
                Prop(LE(Id(vin), RealVal(1.0))));
    }

    for(auto exit: station->exits)
    {
        std::string name(station->name + "_to_" + exit->name + "_0");

        auto vout = new Variable(new Real(),
                                 new Name(name),
                                 chase::generic);

        Fout.push_back(vout);
        c->addDeclaration(vout);

        if(zero_outflow_sum == nullptr) zero_outflow_sum = Id(vout);
        else zero_outflow_sum = Sum(zero_outflow_sum, Id(vout));
    }

    Value * sinkedflow = nullptr;
    for(size_t i = 0; i < warehouse->products.size(); ++i) {
        unsigned int flow = i + 1;

        auto sinked = new Variable(new Real(),
                                    new Name("sinked_" + std::to_string(flow)),
                                    chase::generic);

        c->addDeclaration(sinked);
        Sinked.push_back(sinked);

        Value *inflow = nullptr;
        Value *outflow = nullptr;

        for (auto entry: station->entries) {
            std::string entry_name(entry->name);
            std::string exit_name(station->name);

            std::string name(entry_name + "_to_"
                             + exit_name + "_" + std::to_string(flow));

            auto cvin = new Variable(new Real(), new Name(name),
                                     chase::generic);

            Fin.push_back(cvin);
            c->addDeclaration(cvin);

            if (inflow == nullptr) inflow = Id(cvin);
            else inflow = Sum(inflow, Id(cvin));

            assumptions->addOperand(
                    Prop(GE(Id(cvin), RealVal(0.0))));
            assumptions->addOperand(
                    Prop(LE(Id(cvin), RealVal(1.0))));

            guarantees->addOperand(Prop(Eq(Id(cvin), Id(sinked))));

            if(sinkedflow == nullptr) sinkedflow = Id(sinked);
            else sinkedflow = Sum(sinkedflow, Id(sinked));

        }

        for (auto exit: station->exits) {
            std::string name(station->name + "_to_" + exit->name + "_" + std::to_string(flow));

            auto cvout = new Variable(new Real(), new Name(name),
                                      chase::generic);

            Fout.push_back(cvout);
            c->addDeclaration(cvout);

            if (outflow == nullptr) outflow = Id(cvout);
            else outflow = Sum(outflow, Id(cvout));

            guarantees->addOperand(
                    Prop(Eq(Id(cvout), RealVal(0.0))));

        }
    }

    auto sum_in = reinterpret_cast<Value*>(Id(Fin[0]));
    auto sum_out = reinterpret_cast<Value*>(Id(Fout[0]));

    for(size_t i = 1; i < Fin.size(); ++i) {
        sum_in = reinterpret_cast<Value*>(Sum(sum_in, Id(Fin[i])));
    }

    for(size_t i = 1; i < Fout.size(); ++i) {
        sum_out = reinterpret_cast<Value*>(Sum(sum_out, Id(Fout[i])));
    }

    sinkedflow = Sum(sinkedflow, Id(Fin[0]));
    auto sum_sinked = Prop(Eq(Id(Fout[0]), sinkedflow));

    assumptions->addOperand(Prop(LE(sum_in, RealVal(1.0))));
    assumptions->addOperand(Prop(GE(sum_in, RealVal(0.0))));

    guarantees->addOperand(Prop(LE(sum_out, RealVal(1.0))));
    guarantees->addOperand(Prop(GE(sum_out, RealVal(0.0))));
    guarantees->addOperand(Prop(Eq(sum_in, sum_out)));
    guarantees->addOperand(sum_sinked);

    c->addAssumptions(chase::constraints, assumptions);
    c->addGuarantees(chase::constraints, guarantees);

    if(_params->verbose)
        std::cout << c->getString() <<std::endl;

    return c;
}




#pragma clang diagnostic pop