#include "../include/argparse.h"
#include "./algo/algo.h"

int main (int argc, char **argv) {
    argparse::ArgumentParser program("main");
    argparse::ArgumentParser sse_command("sse", "1.0", argparse::default_arguments::none);
    argparse::ArgumentParser sa_command("sa", "1.0", argparse::default_arguments::none);

    program.add_subparser(sse_command);
    program.add_subparser(sa_command);

    try {
        program.parse_known_args(argc, argv);
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
        std::cout << program;
        return 1;
    }

    std::string algo = "none";
    if (program.is_subcommand_used("sse")) {
        std::cout << "SSE" << std::endl;
        algo = "sse";
    } else if (program.is_subcommand_used("sa")) {
        std::cout << "SA" << std::endl;
        algo = "sa";
    }

    // std::cout << "Algo: " << program.get<std::string>("algo") << std::endl;

    // printf("Iter: %d\n", iter);
    int iter = 2;
    const int ret = execute(argc, argv, iter, algo);
    if (ret == 1) {
        std::cout << "Error: " << ret << std::endl;
    }

    return 0;
}
