/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <sstream>
#include "Console.hh"
#include "utilities/simplify.hh"

using namespace ltl_tool;
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
    if( c1 == nullptr || c2 == nullptr ) {
        messageWarning("Impossible to find the contracts.");
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
    if (c1 == nullptr || c2 == nullptr) {
        messageWarning("Impossible to find the contracts.");
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

    if(solver != "slugs" && solver != "gr1c") {
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

    if(solver == "slugs") {
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
    NuSMVPrinter printer;
    printer.print(contract, fileOut);
    messageInfo(
            "NuSMV specification for synthesis stored in file: " + fileOut);
    return 1;
}

int Console::_execShow(std::vector<std::string> &tokens)
{
    std::string ret("");
    if(tokens.size() == 1)
        ret = _system->getString();
    else {
        for (size_t it = 1; it < tokens.size(); ++it) {
            std::string name = tokens[it];
            bool found = false;
            for(auto & cit :  _system->getContractsSet() )
            {
                if(cit->getName()->getString() == name)
                {
                    found = true;
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