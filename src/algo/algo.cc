#include "./algo.h"
#include <stdarg.h>
#include <string>
#include <unistd.h>
// #include <variant>

int execute (int argc, char **argv, int& iter, std::string algo) {
    // clap::Clap clap;
    // clap.parse(argc, argv, iter);

    // if (clap.hasArg("file")) {
    //     std::string filename = std::get<std::string>(clap.getArg("file")[0]);
    //     printf("File: %s\n", filename.c_str());
    // }

    // const std::string algo = clap.getPosArg(1);
    // printf("Algo: %s\n", algo.c_str());

    const std::string sa_exe  = "./bin/sa";
    const std::string sse_exe = "./bin/sse";

    const int child_argc = argc - iter;
    char **child_argv    = new char *[child_argc + 2];

    child_argv[0] = new char[sa_exe.length() + 1];
    for (int i = 0; i < sa_exe.length(); ++i) {
        child_argv[0][i] = sa_exe[i];
    }

    for (int i = 0; i < child_argc; ++i) {
        child_argv[i + 1] = argv[iter + i];
    }
    child_argv[child_argc + 1] = NULL;

    printf("Running: ");
    for (int i = 0; i < child_argc + 2; ++i) {
        printf("%s ", child_argv[i]);
    }
    printf("\n");

    // printf("Iter: %d\n", iter);
    if (algo == "sa") {
        printf("Simulated Annealing\n");
        // clap.setParams(saParams);
        // clap.parse(argc, argv, iter);
        // if (clap.hasArg("help")) {
        //     clap.help();
        //     return 0;
        // }

        if (execv(sa_exe.c_str(), child_argv) == -1) {
            perror("execv");
            return 1;
        }
    } else if (algo == "sse") {
        printf("Simulated Synchronous Annealing\n");
        // clap.setParams(sseParams);
        // clap.parse(argc, argv, iter);
        // if (clap.hasArg("help")) {
        //     clap.help();
        //     return 0;
        // }

        if (execv(sse_exe.c_str(), child_argv) == -1) {
            perror("execv");
            return 1;
        }
    } else {
        printf("Unknown algorithm\n");
        return 1;
    }
    return 0;
}
