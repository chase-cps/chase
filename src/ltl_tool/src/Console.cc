/**
    * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <sstream>
#include "Console.hh"
#include "utilities/simplify.hh"

using namespace ltl_tool;
using namespace chase;

ltl_tool::Console::Console(System *system) :
    _system(system)
{
    simplify(_system);
}

int Console::run()
{
    std::cout << std::endl << "$> ";
    std::string cmd;

    std::getline(std::cin, cmd);

    if(cmd == "exit") return 0;
    if(cmd == "show")
        std::cout << _system->getString() << std::endl;
    else {
        int rv = _execCommand(cmd);
        if( rv == 0 )
            std::cout << "Wrong command" << std::endl;
    }

    simplify(_system);
    std::cout << _system->getString() << std::endl;

    return 1;
}

int Console::_execCommand(std::string cmd)
{
    // Tokenize.
    std::stringstream ss(cmd);
    std::vector< std::string > tokens;
    std::string intermediate;
    while(std::getline(ss, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    if(tokens[0] == "saturate") {
        if (tokens.size() != 2)
            return 0;
        std::string contract_name = tokens[1];
        for (auto i = _system->getContractsSet().begin();
             i != _system->getContractsSet().end(); ++i)
        {
            Contract * c = *i;
            if(c->getName()->getString() == contract_name )
                Contract::saturate(c);
        }
    }
    else if(tokens[0] == "compose")
    {
        if(tokens.size() != 4)
            return 0;
        std::string c1_name = tokens[1];
        std::string c2_name = tokens[2];
        Contract * c1 = nullptr;
        Contract * c2 = nullptr;
        for (auto i = _system->getContractsSet().begin();
             i != _system->getContractsSet().end(); ++i)
        {
            Contract * c = *i;
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
        if( c1 == nullptr || c2 == nullptr ) return 0;
        names_projection_map m;
        _parseProjectionMap(m);

        auto r = Contract::composition(c1, c2, m, tokens[3]);
        _system->addContract(r);
    }
    else if(tokens[0] == "conjunction")
    {
        if(tokens.size() != 4)
            return 0;
        std::string c1_name = tokens[1];
        std::string c2_name = tokens[2];
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
        if( c1 == nullptr || c2 == nullptr ) return 0;
        names_projection_map m;
        _parseProjectionMap(m);

        auto r = Contract::conjunction(c1, c2, m, tokens[3]);
        _system->addContract(r);
    }
    else if(tokens[0] == "synthesize")
    {
        std::string fileOut = "outuput.structuredSlugs";
        if(tokens.size() > 2)
            fileOut = std::string(tokens[2]);
        if(tokens.size() < 2)
            messageWarning("Wrong command. Usage: synthesize contract file");

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

        VarsCausalityVisitor varsCausality(contract);
        contract->accept_visitor(varsCausality);
        SlugsPrinter printer;
        printer.print(contract, fileOut);
    }


    return 1;
}

void Console::_parseProjectionMap(names_projection_map &m)
{
    std::cout << "INSERT THE PROJECTION MAP. Insert END when finished" << std::endl;
    std::string line;
    while(std::getline(std::cin, line))
    {
        if(line == "END") return;
        std::stringstream ss(line);
        std::vector< std::string > tokens;
        std::string intermediate;
        while(std::getline(ss, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }
        if(tokens.size() == 2)
        {
            std::pair< std::string, std::string > p(tokens[0], tokens[1]);
            m.insert(p);
        } else {
            messageWarning("Wrong format in projection.");
        }
    }
}
