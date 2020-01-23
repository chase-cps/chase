/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 *              This project is released under the 3-Clause BSD License.
 * */

#include "main.hh"

using namespace ltl_tool;
using namespace chase;

int main( int argc, char * argv[] )
{
    auto params = parseCmdLine(argc, argv);

    LTLSpecsBuilder builder;
    builder.parseSpecificationFile(params->fileIn);

}

Params * ltl_tool::parseCmdLine(int argc, char **argv) {
    auto *parameters = new Params();

    opterr = 0;
    int c;

    while ((c = getopt(argc, argv, "i:o:b:V")) != -1) {
        switch (c) {
            case 'i':
                parameters->fileIn = std::string(optarg);
                break;
            case 'o':
                parameters->fileOut = std::string(optarg);
                break;
            case '?':
                printHelp();
                exit(-1);
            case 'V':
                parameters->verbose = true;
                break;
            default:
                printHelp();
                exit(-1);
        }
    }

    if( parameters->fileOut.empty() )
    {
        size_t lastindex = parameters->fileIn.find_last_of('.');
        size_t firstindex = parameters->fileIn.find_last_of('/');
        if( firstindex > 0 ) ++firstindex;
        std::string rawname = parameters->fileIn.substr(firstindex, lastindex-firstindex);

        rawname += ".txt";

        parameters->fileOut = rawname;
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

void ltl_tool::printHelp()
{
    std::cerr << "[USAGE]\n" <<
              "ltl_tool -i input_file [-o output_file] [-V]"
              << std::endl <<
              std::endl <<
              "\t-i : specifies the txt input file containing the specifications."
              << std::endl <<
              "\t-o : specifies the txt output file. Default: input file with .py"
              "\n\t\trather than .txt extension." << std::endl <<
              "\t-V : activate the verbose mode." << std::endl;
}