#include "des.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"

//INSERTED
/**
 * Numerical Association Rule Mining using DE, PSO...
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "DESolver.h"
#include "Setup.h"
#include "Archive.h"
#include "Problem.h"
#include "Squash.h"

//INSERTED

namespace algos {

DES::DES() : NARAlgorithm({}) {
    using namespace config::names;
    RegisterOptions();
}

void DES::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    DifferentialStrategy default_strategy = DifferentialStrategy::rand1Exp;
    RegisterOption(Option{&population_, kPopulationSize, kDPopulationSize, 100});
    RegisterOption(Option{&num_evaluations_, kMaxFitnessEvaluations, kDMaxFitnessEvaluations, 1000});
    RegisterOption(Option{&differential_scale_, kDifferentialScale, kDDifferentialScale, 0.5});
    RegisterOption(Option{&crossover_probability_, kCrossoverProbability, kDCrossoverProbability, 0.9});
    RegisterOption(Option{&differential_strategy_, kDifferentialStrategy, kDDifferentialStrategy, default_strategy});
}

void DES::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kPopulationSize, kMaxFitnessEvaluations, kDifferentialScale,
                         kCrossoverProbability, kDifferentialStrategy});
}

void help(FILE *stream) {
    fprintf(stream, "uARMSolver version 0.3.0 (July 2024)\n\n");
    fprintf(stream, "Syntax\n");
    fprintf(stream, "  uARMSolver [-v|-?] [-s'arm.set'|-s 'arm.set']\n");
}

/**
 * Call the appropriate ARM problem solver.
 *
 * @param incorporate parameter setup, problem definition and produced archive of association rules.
 * @return no return value.
 */
void solve(Setup setup, Problem problem, Archive &rules) {
    clock_t start_t, end_t;    // time measuring in miliseconds

    start_t = clock();
    cout << "Solver strated..." << endl;
    switch (setup.get_solver()) {
        case SOLVER_DE: {
            int n_dim = problem.get_dimension();
            int n_np = setup.get_Np();
            int n_run = setup.get_RUNs();

            cout << "n_dim= " << n_dim << ", n_np= " << n_np << endl;
            DESolver solver(n_dim, n_np, problem);
            cout << "Setup..." << endl;
            solver.Setup(setup.alg_param.de.strategy, setup.alg_param.de.scale, setup.alg_param.de.xover);
            for (int i = 0; i < n_run; i++) {
                int n_fes = setup.get_FEs();
                cout << "n_fes= " << n_fes << endl;
                solver.Evolve(i, n_fes, rules);
            }
        }
            break;
        case SOLVER_PSO:
            break;
    }

    end_t = clock();
    printf("Total time= %lf\n", (double) (end_t - start_t) / CLOCKS_PER_SEC);
}

/**
 * Prepare data needed for visualization - not implemented yet.
 *
 * @param incorporate parameter setup, and archive of association rules.
 * @return no return value.
 */
void visualize(Setup set, Archive rules) {

}

unsigned long long DES::GenerateAllNARs() {
    //TODO
    return 0;
}

unsigned long long DES::ExecuteInternal() {

    //MAIN FUNCTION OF DESOLVER
    int argc = 0;
    char *argv[] = {nullptr};

    auto time = (long long) 0;

    string s_name = "arm.set";

    for (int i = 1; i < argc; i++) {
        if ((strncmp(argv[i], "-v", 2) == 0) || (strncmp(argv[i], "-?", 2) == 0)) {
            //used to call help() here
            return EXIT_FAILURE;
        } else if (strncmp(argv[i], "-s", 2) == 0)    // setup file name
        {
            s_name.assign(&argv[i][2]);
            // allow -s FILE, not just -sFILE:
            if (s_name.empty() && i < argc - 1) {
                s_name.assign(argv[++i]);
            }
        } else {
            fprintf(stderr, "Unexpected argument: %s\n\n", argv[i]);
            help(stderr);
            return EXIT_FAILURE;
        }
    }

    srand(1);

    printf("ARM setup: setup_file_name = %s.\n", s_name.c_str());

    // read setup file
    Setup setup(s_name);

    if (!setup.read()) {
        printf("ARM setup: File %s not read.\n", s_name.c_str());
        exit(-1);
    }

    setup.print_param();

    // problem definition - read transaction database(s)
    Problem prob[setup.get_period()];

    for (int i = 0; i < setup.get_period(); i++) {    // number of problems
        string str = setup.get_tdbase_name();
        if (setup.get_period() > 1) {
            char f_name[256];
            sprintf(f_name, ".%d", i + 1);
            str.append(f_name);
        }

        printf("Reading transaction database= %d, f_name= %s...\n", i + 1, str.c_str());
        // read transaction database(s)
        prob[i].init_tdbase(setup, str);

        // squashing database if needed
        if (setup.get_squash() > 0) {
            Squash sq;
            sq.make_sq_dbase(setup, prob[i]);
            sq.write(setup);
            sq.stat(prob[i]);
            prob[i].dbase.clear();
            prob[i].dbase = sq.sq_dbase;
            sq.sq_dbase.clear();
        }
    }

    Archive rules[setup.get_period()];

    if (setup.get_solver() == SOLVER_NONE) {        // reading the existing ARM archive
        for (int i = 0; i < setup.get_period(); i++) {    // number of periods
            string str = setup.get_rule_name();
            if (setup.get_period() > 1) {
                char f_name[256];
                sprintf(f_name, ".%d", i + 1);
                str.append(f_name);
            }

            printf("Reading file= %d, f_name= %s...\n", i + 1, str.c_str());
            // read transaction database(s)
            rules[i].read(str, prob[i]);
        }
    } else {        // producing the new ARM archive
        for (int i = 0; i < setup.get_period(); i++) {    // number of periods
            // evaluate solution
            solve(setup, prob[i], rules[i]);
            // obtain output file name
            string str = setup.get_out_name();
            if (setup.get_period() > 1) {
                char f_name[256];
                sprintf(f_name, ".%d", i + 1);
                str.append(f_name);
            }
            printf("Writing file= %d, f_name= %s...\n", i + 1, str.c_str());
            // write archive of rules found
            rules[i].asort();    // sort the archive
            rules[i].write(str, prob[i]);
        }
    }
    exit(-1);

    // visualization
    visualize(setup, rules[0]);

    return 0;

    return time;
}

}