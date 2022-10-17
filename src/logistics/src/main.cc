/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/18/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "main.hh"
#include "z3++.h"
#include "stdlib.h"
using namespace z3;
using namespace chase;

int main( int argc, char * argv[] ) {

    auto params = parseCmdLine(argc, argv);
    LogisticsSpecsBuilder builder(params);
    builder.parseSpecificationFile(params->fileIn);
}

Params * chase::parseCmdLine(int argc, char **argv) {
    auto *parameters = new Params();

    opterr = 0;
    int c;

    while ((c = getopt(argc, argv, "i:c:b:o:t:V")) != -1) {
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
                break;
            case 'V':
                parameters->verbose = true;
                break;
            case 't':
                parameters->timesteps = atoi(std::string((optarg)).c_str());
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

    return parameters;
}

void chase::printHelp()
{
    std::cerr << "[USAGE]\n" <<
              "chase -i input_file -c commands_file [-V]"
              << std::endl <<
              std::endl <<
              "\t-t : specifies the number of timesteps. Default is 100."
              << std::endl <<
              "\t-i : specifies the txt input file containing the specifications."
              << std::endl <<
              "\t-c : command file to be executed."
              << std::endl <<
              "\t-o : output directory path."
              << std::endl <<
              "\t-V : activate the verbose mode." << std::endl;
}