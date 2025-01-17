#include "../include/clap.h"
#include "./algo/algo.h"
// #include <iostream>

// clang-format off
const std::string clapParams = 
    "Program: main\n"
    "Version: 0.1.0\n"
    "Description: Digital Annealer V2\n"
    "-h, --help Show help message\n"
    "<command> Either 'sa' or 'sse'. For more information for both method, use `./main <method> --help`\n";
// clang-format on

int main (int argc, char **argv) {
    clap::Clap clap(clapParams);
    int iter;
    clap.parse(argc, argv, iter, 1);

    std::string algo = clap.getPosArg(0);
    // printf("Algo: %s\n", algo.c_str());

    if (clap.hasArg("help")) {
        clap.help();
        return 0;
    }

    // printf("Iter: %d\n", iter);
    const int ret = execute(argc, argv, iter, algo);
    if (ret == 1) {
        std::cout << "Error: " << ret << std::endl;
    }

    return 0;
}
