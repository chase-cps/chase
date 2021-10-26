/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <sstream>
#include "Console.hh"
#include "utilities/simplify.hh"

using namespace chase;
using namespace chase;

chase::Console::Console(System *system, std::string outDir) :
    _system(system), _outDir(outDir)
{
    simplify(_system);
}

int Console::run(std::string cmd)
{
    // Identify the type of contracts in the specification.

    LogicIdentificationVisitor logicIdVisit;
    for(auto & cit :  _system->getContractsSet())
    {
        auto t = logicIdVisit.identifyContractType(cit);
        auto it = _used_logics.find(cit);
        if (it != _used_logics.end()) { it->second = t; }
        else
            _used_logics.insert(std::pair<Contract *, logics_type>(cit, t));
    }

    if(cmd.empty()) {
        std::cout << std::endl << "$> ";
        std::getline(std::cin, cmd);
        if(cmd.empty()) return 1;
    }

    if(cmd == "exit") return 0;
    else {
        int rv = _execCommand(cmd);
        if( rv == 0 )
            std::cout << "Wrong command:\n\t" << cmd
            << "\n\tHINT:\tType help to see instructions.\n" <<
            std::endl;
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
        tokens.push_back(intermediate);

    if(tokens[0] == "help")
    {
        if(tokens.size() > 1)
        {
            cmd = tokens[1];
            _printHelp(cmd);
            return 1;
        }
        else
        {
            _printHelp();
            return 1;
        }
    }
    else if(tokens[0] == "saturate")
        return _execSaturation(tokens);
    else if(tokens[0] == "compose")
        return _execComposition(tokens);
    else if(tokens[0] == "conjunction")
        return _execConjunction(tokens);
    else if(tokens[0] == "synthesize")
        return _execSynthesis(tokens);
    else if(tokens[0] == "verify")
        return _execVerification(tokens);
    else if(tokens[0] == "refinement")
        return _checkRefinement(tokens);
    else if(tokens[0] == "quotient")
        return _computeQuotient(tokens);
    else if(tokens[0] == "show")
        return _execShow(tokens);
    else if(tokens[0] == "stl")
        return _stlContract(tokens);
    return 0;
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
        std::string contracts = "";
        for (auto & c : _system->getContractsSet())
        {
            contracts += "\t- " + c->getName()->getString() + "\n";
        }
        std::cout
                << "Contracts:\n" << contracts
                << std::endl
                << "Commands:" << std::endl
                << "\tshow\t\t: print to screen the system, or a contract."
                << std::endl
                << "\tcompose\t\t: compute the composition of two contracts."
                << std::endl
                << "\tconjunction\t: compute the conjunction of two contracts."
                << std::endl
                << "\tsaturate\t: saturate a contract."
                << std::endl
                << "\tsynthesize\t: synthesize a contract."
                << std::endl
                << "\tverify\t\t: verify compatibility and consistency of a contract."
                << std::endl
                << "\trefinement\t\t: check whether a contract refines another."
                << std::endl;
    }
    else if(cmd == "show")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" [contract]\n")
                  << "\t\tcontract:\t\tfirst contract to print to screen (optional).\n"
                  << std::endl;
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
                  cmd + std::string(" contract_1 contract_2 result mode\n")
                  << "\t\tcontract_1:\t\tfirst contract to conjoin.\n"
                  << "\t\tcontract_2:\t\tsecond contract to conjoin.\n"
                  << "\t\tresult:\t\tcontract generated by the conjunction.\n"
                  << "\t\tmode:\t\t\tbased on name or map.\n"
                  << std::endl;
    else if(cmd == "quotient")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract_1 contract_2 result mode synthesizable\n")
                  << "\t\tcontract_1:\t\tdividend contract.\n"
                  << "\t\tcontract_2:\t\tdivisor contract.\n"
                  << "\t\tresult:\t\tcontract generated by the quotient.\n"
                  << "\t\tmode:\t\t\tbased on name or map.\n"
                  << "\t\tsynthesizable:\t\ttrue if you whish to produce a GR(1) specification."
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
                  << "\t\tsolver:\t\tmight be either slugs, gr1c or all (default, slugs).\n"
                  << std::endl;
    else if(cmd == "verify")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract file\n")
                  << "\tcontract:\t\tthe contract to verify.\n"
                  << "\tfile:\t\tfile generated for the solver.\n"
                  << std::endl;
    else if(cmd == "refinement")
        std::cout << "Usage:\n\t$> " <<
                  cmd + std::string(" contract1 contract2 file mode\n")
                  << "\tThe operation checks whether contract1 refines contract 2."
                  << "\tcontract 1:\t\tthe first contract.\n"
                  << "\tcontract 2:\t\tthe first contract.\n"
                  << "\tfile:\t\tfile generated for the solver.\n"
                  << "\tmode: \t\t\tthe merge of variables is based on name or map.\n"
                  << std::endl;
    else
        std::cout << "Wrong command." << std::endl;
}












