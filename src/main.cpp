#include <iostream>
#include <memory>
#include "Nani.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "USAGE: " << argv[0] << " <file.json>";
        return 1;
    }

    Nani nani(argv[1]);
    nani.ConvertToNani();

    return 0;
}