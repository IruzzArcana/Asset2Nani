#include <iostream>
#include <memory>
#include "Nani.hpp"
#include "argparse.hpp"

int main(int argc, char **argv)
{
    argparse::ArgumentParser program("asset2nani", VERSION);
    program.add_argument("filename")
        .help("input file (*.json)");
    program.add_argument("--cmd", "-c")
        .default_value("nanicmds.json")
        .help("specify nanicmd map file (*.json)");
    program.add_argument("--output", "-o")
        .default_value("")
        .help("output file (*.nani)");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    try
    {
        Nani nani(program.get<std::string>("filename"), program.get<std::string>("--cmd"), program.get<std::string>("-o"));
        nani.ConvertToNani();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
        return 1;
    }

    return 0;
}