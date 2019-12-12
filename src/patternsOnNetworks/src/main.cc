/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 *              This project is released under the 3-Clause BSD License.
 * */

#include "main.hh"

using namespace patternsOnNetworks;
using namespace chase;

int main( int argc, char * argv[] )
{
    Params * parameters = parseCmdLine(argc, argv);
    if(parameters == nullptr) return -1;

    // Start parsing the input file.
    SpecBuilder builder;
    builder.parseSpecificationFile(parameters->fileIn);
    Contract * c = builder.getProblem()->getContract();

    GR1CPrinter printer;
    printer.print(c, parameters->fileOut);

    std::cout << c->getString() << std::endl;

    delete(c);

    return 0;
}



void patternsOnNetworks::printHelp()
{
    std::cerr << "[USAGE]\n" <<
              "patternsToNetworks -i input_file [-o output_file] [-V]"
              << std::endl <<
              std::endl <<
              "\t-i : specifies the txt input file containing the specifications."
              << std::endl <<
              "\t-o : specifies the txt output file. Default: input file with .py"
              "\n\t\trather than .txt extension." << std::endl <<
              "\t-V : activate the verbose mode." << std::endl;
}

Params * patternsOnNetworks::parseCmdLine( int argc, char * argv[] )
{
    auto * parameters = new Params();

    opterr = 0;
    int c;

    while(( c = getopt(argc, argv, "i:o:V")) != -1 )
    {
        switch(c)
        {
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
        rawname += ".spc";
        parameters->fileOut = rawname;
    }

    std::ifstream f(parameters->fileIn.c_str());
    if( ! f.good() )
    {
        printHelp();
        f.close();
        exit(-1);
    }
    f.close();
    return parameters;
}