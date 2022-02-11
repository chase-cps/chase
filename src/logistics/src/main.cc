/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/18/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "main.hh"
#include "z3++.h"

using namespace z3;
using namespace chase;

int main( int argc, char * argv[] ) {

    // Test Z3
    std::cout << "de-Morgan example\n";

    context c;

    expr x = c.bool_const("x");
    expr y = c.bool_const("y");
    expr op1 = x;
    expr conjecture = (!(op1 && y)) == (!x || !y);
    expr conjecture2 = (!(x && y)) == (!x || !y) && conjecture;

    solver s(c);
    // adding the negation of the conjecture as a constraint.
    s.add(!conjecture2);
    std::cout << s << "\n";
    std::cout << s.to_smt2() << "\n";
    switch (s.check()) {
        case unsat:   std::cout << "de-Morgan is valid\n"; break;
        case sat:     std::cout << "de-Morgan is not valid\n"; break;
        case unknown: std::cout << "unknown\n"; break;
    }
    // End test z3.


    auto params = parseCmdLine(argc, argv);
    LogisticsSpecsBuilder builder;
    builder.parseSpecificationFile(params->fileIn);
    messageInfo("Phase 2: building contract-based representation.");
    messageInfo("Phase 3: encoding.");
    messageInfo("Phase 4: solution.");
}

Params * chase::parseCmdLine(int argc, char **argv) {
    auto *parameters = new Params();

    opterr = 0;
    int c;

    while ((c = getopt(argc, argv, "i:c:b:o:V")) != -1) {
        switch (c) {
            case 'i':
                parameters->fileIn = std::string(optarg);
                break;
            case 'c':
                parameters->cmdFile = std::string(optarg);
                break;
            case '?':
                printHelp();
                exit(-1);
            case 'o':
                parameters->outDir = std::string(optarg);
                if(parameters->outDir.back() != '/')
                    parameters->outDir += "/";
                break;
            case 'V':
                parameters->verbose = true;
                break;
            default:
                printHelp();
                exit(-1);
        }
    }

    std::ifstream f(parameters->fileIn.c_str());
    if( ! f.good() )
    {
        printHelp();
        f.close();
        std::cout << "fileIn: " << parameters->fileIn.c_str() << std::endl;
        exit(-1);
    }
    f.close();

    struct stat info;
    if(stat (parameters->outDir.c_str(), &info) != 0 && !parameters->outDir
            .empty()) {
        if (mkdir(parameters->outDir.c_str(), 0755) == -1)
        {
            std::cout << strerror(errno) << std::endl;
            exit(-1);

        } else messageInfo(parameters->outDir + " created.");
    }
    return parameters;
}

void chase::printHelp()
{
    std::cerr << "[USAGE]\n" <<
              "chase -i input_file -c commands_file [-V]"
              << std::endl <<
              std::endl <<
              "\t-i : specifies the txt input file containing the specifications."
              << std::endl <<
              "\t-c : command file to be executed."
              << std::endl <<
              "\t-o : output directory path."
              << std::endl <<
              "\t-V : activate the verbose mode." << std::endl;
}