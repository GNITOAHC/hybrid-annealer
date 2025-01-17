#pragma once
#include "../../include/clap.h"
#include <string>

const std::string algoClapParams = "-f, --file <filename:string> The input file to read\n";

const std::string saParams =
    "Description: Simulated Annealing\n"
    "--help                    Show help message\n"
    "--file <path:string>      The input file to read\n"
    "--h-tri <length:int>      Use built-in tool to create triangular lattice\n"
    "--ini-g <gamma:int>       Specify an initial gamma value for triangular lattice\n"
    "--final-g <gamma:double>  Specify an final gamma value for triangular lattice\n"
    "--ini-t <temp:double>     Specify an initial temperature value for triangular lattice\n"
    "--ini-t <temp:double>     Specify an initial temperature value for triangular lattice\n"
    "--final-t <temp:double>   Specify an final temperature value for triangular lattice\n"
    "--tau <tau:int>           Specify a tau for annealer\n"
    "--func <func:string>      Specify a function for annealer, either 'sa' or 'sqa'\n"
    "--print-progress          Print the annealing progress\n"
    "--print-conf              Output the configuration\n";

const std::string sseParams = "-h, --help                   Display this help and exit\n"
                              "-q, --qubo                   Use QUBO format\n"
                              "-f, --file <path:string>     Input file path\n"
                              "-t, --init-t <temp:double>   Initial temperature\n"
                              "-T, --final-t <temp:double>  Final temperature\n"
                              "-h, --init-hx <hx:double>    Initial Hamiltonian\n"
                              "-H, --final-hx <hx:double>   Final Hamiltonian\n"
                              "-x, --tau <tau:int>          Tau\n"
                              "-p, --print-conf             Print final configuration\n"
                              "-l. --tri-l <length:int>     Default triangular setup\n";

int execute(int argc, char **argv, int& iter, std::string algo);
