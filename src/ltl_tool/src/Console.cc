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

int Console::run(std::string cmd)
{
    if(cmd.empty()) {
        std::cout << std::endl << "$> ";
        std::getline(std::cin, cmd);
        if(cmd.empty()) return 1;
    }

    if(cmd == "exit") return 0;
    if(cmd == "show")
        std::cout << _system->getString() << std::endl;
    else {
        int rv = _execCommand(cmd);
        if( rv == 0 )
            std::cout << "Wrong command" << std::endl;
    }
    simplify(_system);
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

    if(tokens[0] == "help")
    {
        std::string cmd;
        if(tokens.size() > 1) cmd = tokens[1];
        _printHelp(cmd);
    }
    else if(tokens[0] == "saturate") {
        if (tokens.size() != 2)
            return 0;
        std::string contract_name = tokens[1];
        for (auto c : _system->getContractsSet())
        {
            if(c->getName()->getString() == contract_name )
                Contract::saturate(c);
        }
    }
    else if(tokens[0] == "compose")
    {
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
    }
    else if(tokens[0] == "conjunction") {
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
    }
    else if(tokens[0] == "synthesize")
    {
        std::string fileOut = "output.structuredSlugs";
        std::string solver = "slugs";
        if(tokens.size() > 2)
            fileOut = std::string(tokens[2]);
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

        VarsCausalityVisitor varsCausality(contract);
        contract->accept_visitor(varsCausality);

        if(solver == "slugs") {
            if (fileOut.find("structuredSlugs") == std::string::npos)
                fileOut += ".structuredSlugs";
            SlugsPrinter printer;
            printer.print(contract, fileOut);
            messageInfo(
                    "SLUGS specification for synthesis stored in file: " + fileOut);
        }
        else if(solver == "gr1c") {
            if (fileOut.find(".spc") == std::string::npos)
                fileOut += ".spc";
            GR1CPrinter printer;
            printer.print(contract, fileOut);
            messageInfo(
                    "GR1C specification for synthesis stored in file: " + fileOut);
        }
        else
        {
            messageWarning("Unsupported solver.");
            return 1;
        }

    }
    else if(tokens[0] == "verify")
    {
        std::string fileOut = "output.smv";
        if(tokens.size() > 2)
            fileOut = std::string(tokens[2]);
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

    }
    return 1;
}

void Console::_createProjectionMap(
        names_projection_map &m, std::string mode,
        Contract * c1, Contract * c2)
{
    if(mode == "manual")
        _createProjectionMapManual(m);
    if(mode == "name")
        _createProjectionMapName(m, c1, c2);
    else
        _createProjectionMapFile(m, mode);
}

void Console::_createProjectionMapManual(names_projection_map &m)
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

void Console::_createProjectionMapName(
        names_projection_map & m, Contract * c1, Contract * c2)
{
    for(auto & dec1 : c1->declarations)
    {
        if(dec1->IsA() != variable_node) continue;
        for(auto & dec2 : c2->declarations ) {
            if (dec2->IsA() != variable_node) continue;

            std::string n1 = dec1->getName()->getString();
            std::string n2 = dec2->getName()->getString();
            if(n1 == n2)
                m.insert(std::pair<std::string, std::string>(n1, n2));
        }
    }
}

void Console::_createProjectionMapFile(
        names_projection_map &m, std::string file)
{
    std::ifstream ifile(file);
    std::string line;
    while(std::getline(ifile, line))
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


void Console::_printHelp(std::string cmd) {
    if(cmd.empty()) {
        std::cout
                << "Commands:" << std::endl
                << "\tcompose\t\t: compute the composition of two contracts."
                << std::endl
                << "\tconjunction\t: compute the conjunction of two contracts."
                << std::endl
                << "\tsaturate\t: saturate a contract."
                << std::endl
                << "\tsynthesize\t: synthesize a contract."
                << std::endl
                << "\tverify\t\t: verify compatibility and consistency of a contract."
                << std::endl;
    }
    else if(cmd == "compose")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract_1 contract_2 result mode\n")
                  << "\t\tcontract_1:\t\tfirst contract to compose.\n"
                  << "\t\tcontract_2:\t\tsecond contract to compose.\n"
                  << "\t\tresult:\t\t\tcontract generated by the composition.\n"
                  << "\t\tmode:\t\t\tbased on name or map.\n"
                  << std::endl;
    else if(cmd == "conjunction")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract_1 contract_2 resulti mode\n")
                  << "\t\tcontract_1:\t\tfirst contract to conjoin.\n"
                  << "\t\tcontract_2:\t\tsecond contract to conjoin.\n"
                  << "\t\tresult:\t\tcontract generated by the conjunction.\n"
                  << "\t\tmode:\t\t\tbased on name or map.\n"
                  << std::endl;
    else if(cmd == "saturate")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract\n")
                  << "\t\tcontract:\t\tthe contract to saturate.\n"
                  << std::endl;
    else if(cmd == "synthesize")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract file solver\n")
                  << "\t\tcontract:\t\tthe contract to synthesize.\n"
                  << "\t\tfile:\t\tfile generated for the solver.\n"
                  << "\t\tsolver:\t\tmight be either slugs or gr1c (default, slugs).\n"
                  << std::endl;
    else if(cmd == "verify")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract file\n")
                  << "\tcontract:\t\tthe contract to verify.\n"
                  << "\tfile:\t\tfile generated for the solver.\n"
                  << std::endl;
    else
        std::cout << "Wrong command." << std::endl;
}
