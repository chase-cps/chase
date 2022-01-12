/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <sstream>
#include "Console.hh"
#include "utilities/simplify.hh"
#include "utilities/Factory.hh"
#include "utilities/ClonedDeclarationVisitor.hh"
#include "backends/PySTLPrinter.hh"

using namespace chase;
using namespace chase;

int Console::_execComposition(std::vector< std::string >& tokens) {
    if(tokens.size() < 4) {
        messageWarning("Wrong command.");
        _printHelp(tokens[0]);

    }
    std::string c1_name = tokens[1];
    std::string c2_name = tokens[2];
    std::string res_name = tokens[3];
    std::string mode("name");
    if(tokens.size() > 4)
        mode = tokens[4];


    Contract * c1 = nullptr;
    Contract * c2 = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if(c->getName()->getString() == c1_name )
        {
            c1 = c;
            continue;
        }
        if(c->getName()->getString() == c2_name )
        {
            c2 = c;
            continue;
        }
    }
    if (c1 == nullptr) {
        messageWarning("Impossible to find the contracts: " + c1_name);
        return 1;
    }
    if (c2 == nullptr) {
        messageWarning("Impossible to find the contracts: " + c2_name);
        return 1;
    }
    names_projection_map m;
    _createProjectionMap(m, mode, c1, c2);

    auto r = Contract::composition(c1, c2, m, tokens[3]);
    _system->addContract(r);
    return 1;
}

int Console::_execConjunction(std::vector<std::string> &tokens) {
    if (tokens.size() < 4) {
        messageWarning("Wrong command.");
        _printHelp(tokens[0]);

    }
    std::string c1_name = tokens[1];
    std::string c2_name = tokens[2];
    std::string res_name = tokens[3];
    std::string mode("name");
    if (tokens.size() > 4)
        mode = tokens[4];


    Contract *c1 = nullptr;
    Contract *c2 = nullptr;
    for (auto c : _system->getContractsSet()) {
        if (c->getName()->getString() == c1_name) {
            c1 = c;
            continue;
        }
        if (c->getName()->getString() == c2_name) {
            c2 = c;
            continue;
        }
    }
    if (c1 == nullptr) {
        messageWarning("Impossible to find the contracts: " + c1_name);
        return 1;
    }
    if (c2 == nullptr) {
        messageWarning("Impossible to find the contracts: " + c2_name);
        return 1;
    }
    names_projection_map m;
    _createProjectionMap(m, mode, c1, c2);

    auto r = Contract::conjunction(c1, c2, m, res_name);
    _system->addContract(r);
    return 1;
}

int Console::_execSaturation(std::vector<std::string> &tokens) {
    if (tokens.size() != 2)
        return 0;
    std::string contract_name = tokens[1];
    for (auto c : _system->getContractsSet())
    {
        if(c->getName()->getString() == contract_name )
            Contract::saturate(c);
    }
    return 1;
}

int Console::_execSynthesis(std::vector<std::string> &tokens) {
    std::string fileOut = _outDir + "output.structuredSlugs";
    std::string solver = "slugs";
    if(tokens.size() > 2)
        fileOut =  _outDir + std::string(tokens[2]);
    if(tokens.size() < 2)
        messageWarning("Wrong command. Usage: synthesize contract file");
    if(tokens.size() > 3)
        solver = std::string(tokens[3]);

    if(solver != "slugs" && solver != "gr1c" && solver != "all") {
        messageWarning("Invalid solver: " + solver);
        return 1;
    }

    std::string contract_name = tokens[1];
    chase::Contract * contract = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if( c->getName()->getString() == contract_name)
        {
            contract = c;
            break;
        }
    }
    if(contract == nullptr) return 1;

    if(solver == "all")
    {
        auto sf = fileOut;
        if (sf.find("structuredSlugs") == std::string::npos)
            sf += ".structuredSlugs";
        auto gf = fileOut;
        if (gf.find(".spc") == std::string::npos)
            gf += ".spc";
        SlugsPrinter sprinter;
        sprinter.print(contract, sf);
        messageInfo(
                "SLUGS specification for synthesis stored in file: " + sf);
        GR1CPrinter gprinter;
        gprinter.print(contract, gf);
        messageInfo(
                "GR1C specification for synthesis stored in file: " + gf);
        return 1;
    }

    else if(solver == "slugs") {
        if (fileOut.find("structuredSlugs") == std::string::npos)
            fileOut += ".structuredSlugs";
        SlugsPrinter printer;
        printer.print(contract, fileOut);
        messageInfo(
                "SLUGS specification for synthesis stored in file: " + fileOut);
        return 1;
    }
    else if(solver == "gr1c") {
        if (fileOut.find(".spc") == std::string::npos)
            fileOut += ".spc";
        GR1CPrinter printer;
        printer.print(contract, fileOut);
        messageInfo(
                "GR1C specification for synthesis stored in file: " + fileOut);
        return 1;
    }
    else
    {
        messageWarning("Unsupported solver.");
        return 1;
    }
}

int Console::_execVerification(std::vector<std::string> &tokens) {
    std::string fileOut = _outDir + "output.smv";
    if(tokens.size() > 2)
        fileOut = _outDir + std::string(tokens[2]);
    if(tokens.size() < 2)
        messageWarning("Wrong command. Usage: verify contract file");

    if(fileOut.find("smv") == std::string::npos)
        fileOut += ".smv";

    std::string contract_name = tokens[1];
    chase::Contract * contract = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if( c->getName()->getString() == contract_name)
        {
            contract = c;
            break;
        }
    }
    if(contract == nullptr) return 1;
    contract = contract->clone();

    auto it = contract->assumptions.find(logic);
    // No assumptions. Surely true.
    if(it == contract->assumptions.end()) {
        return 0;
    }
    // Else...
    auto assumptions = static_cast< LogicFormula * >((*it).second);

    it->second = Not(assumptions);

    it = contract->guarantees.find(logic);
    // No assumptions. Surely true.
    if(it == contract->guarantees.end()) {
        return 0;
    }
    // Else...
    auto guarantees = static_cast< LogicFormula * >((*it).second);

    it->second = Not(guarantees);

    NuSMVPrinter printer(fileOut);
    printer.print(contract);
    messageInfo(
            "NuSMV specification for synthesis stored in file: " + fileOut);
    return 1;
}

int Console::_execShow(std::vector<std::string> &tokens)
{
    std::string ret("");
    if(tokens.size() == 1)
        _system->getString();
    else {
        for (size_t it = 1; it < tokens.size(); ++it) {
            std::string name = tokens[it];
            bool found = false;
            for(auto & cit :  _system->getContractsSet() )
            {
                if(cit->getName()->getString() == name)
                {
                    found = true;
                    auto m = _used_logics.find(cit);
                    if(m != _used_logics.end())
                    {
                        auto t= m->second;
                        switch(t)
                        {
                            case no_logics:
                                ret += "NO LOGICS\n";
                                break;
                            case propositional:
                                ret += "PROPOSITIONAL LOGICS\n";
                                break;
                            case linear_temporal:
                                ret += "LINEAR TEMPORAL LOGICS\n";
                                break;
                            case metric_temporal:
                                ret += "METRIC TEMPORAL LOGICS\n";
                                break;
                            case signal_temporal:
                                ret += "SIGNAL TEMPORAL LOGICS\n";
                                break;
                            default:
                                ret += "NO LOGICS";
                                break;
                        }
                    }

                    ret += cit->getString();
                    ret += "\n\n";
                    break;
                }
            }
            if( found == false )
                messageWarning("Contract not found: " + name);
        }
    }
    std::cout << ret << std::endl;
    return 1;
}

int Console::_checkRefinement(std::vector<std::string> &tokens) {
    std::string fileOut = _outDir + "output.smv";
    if(tokens.size() > 3)
        fileOut = _outDir + std::string(tokens[3]);
    if(tokens.size() < 3) {
        messageWarning("Wrong command. Usage: refinement contract1 contract2 file");
        return 1;
    }

    std::string c1_name = tokens[1];
    std::string c2_name = tokens[2];

    std::string mode("name");
    if(tokens.size() > 4)
        mode = tokens[4];

    chase::Contract * c1 = nullptr;
    chase::Contract * c2 = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if( c->getName()->getString() == c1_name)
            c1 = c;
        if( c->getName()->getString() == c2_name)
            c2 = c;
    }
    if(c1 == nullptr) {
        messageWarning("Contract not found: " + c1_name);
        return 1;
    }
    if(c2 == nullptr) {
        messageWarning("Contract not found: " + c2_name);
        return 1;
    }
    c1 = c1->clone();
    c2 = c2->clone();

    names_projection_map m;
    _createProjectionMap(m, mode, c1, c2);
    auto r = Contract::refinementCheck(c1, c2, m);

    NuSMVPrinter printer(fileOut);
    printer.print(r);
    messageInfo(
            "NuSMV specification for synthesis stored in file: " + fileOut);

    return 1;
}

int Console::_computeQuotient(std::vector<std::string> &tokens) {
    std::string c1_name = tokens[1];
    std::string c2_name = tokens[2];
    std::string res_name = tokens[3];
    std::string mode("name");
    if(tokens.size() > 4)
        mode = tokens[4];
    bool synth = false;
    if(tokens.size() > 5 && tokens[5] == "true")
        synth = true;

    Contract * c1 = nullptr;
    Contract * c2 = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if(c->getName()->getString() == c1_name )
        {
            c1 = c;
            continue;
        }
        if(c->getName()->getString() == c2_name )
        {
            c2 = c;
            continue;
        }
    }
    if( c1 == nullptr || c2 == nullptr ) {
        messageWarning("Impossible to find the contracts.");
        return 1;
    }
    names_projection_map m;
    _createProjectionMap(m, mode, c1, c2);

    auto r = Contract::quotient(c1, c2, m, res_name, synth);
    _system->addContract(r);
    return 1;
}

int Console::_stlContract(std::vector< std::string >& tokens)
{
    std::string fileOut = _outDir + "output.py";
    if(tokens.size() > 2)
        fileOut = _outDir + std::string(tokens[2]);
    if(tokens.size() < 2)
        messageWarning("Wrong command. Usage: stl contract file");


    if(fileOut.find(".py") == std::string::npos)
        fileOut += ".py";

    std::string contract_name = tokens[1];
    chase::Contract * contract = nullptr;
    for (auto c : _system->getContractsSet())
    {
        if( c->getName()->getString() == contract_name)
        {
            contract = c;
            break;
        }
    }

    PySTLPrinter printer(fileOut);
    printer.print(contract);

    return 1;
}



