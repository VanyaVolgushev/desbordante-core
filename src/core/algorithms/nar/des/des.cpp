#include "des.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"
#include "algorithms/nar/des/create_feature.h"

//INSERTED
/**
 * Numerical Association NAR Mining using DE, PSO...
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

//INSERTED

namespace algos::des {

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

    int n_dim = problem.get_dimension();
    int n_np = setup.get_Np();

    cout << "n_dim= " << n_dim << ", n_np= " << n_np << endl;
    DESolver solver(n_dim, n_np, problem);
    cout << "Setup..." << endl;
    solver.Setup(setup.alg_param.de.strategy, setup.alg_param.de.scale, setup.alg_param.de.xover);
    int n_fes = setup.get_FEs();
    cout << "n_fes= " << n_fes << endl;
    solver.Evolve(n_fes, rules);

    end_t = clock();
    printf("Total time= %lf\n", (double) (end_t - start_t) / CLOCKS_PER_SEC);
}

unsigned long long DES::GenerateAllNARs() {
    //TODO
    return 0;
}

void DES::Test() {
    for (uint columnIndex = 0; columnIndex < typed_relation_->GetNumColumns(); ++columnIndex) {
        size_t num_rows = typed_relation_->GetColumnData(columnIndex).GetNumRows();
        size_t row_index = (columnIndex + 14) % num_rows;
        model::TypedColumnData const& column = typed_relation_->GetColumnData(columnIndex);
        const byte* value;

        if(!column.IsNullOrEmpty(row_index))
        {
            value = column.GetValue(row_index);
        }
        int integer_value = model::Type::GetValue<int>(value);
        std::cout << "column " << columnIndex << " " << "\n";
        std::cout << "type: " << column.GetType().ToString();
        std::cout << "example of data at " << row_index << ": ";
        std::cout <<  integer_value;
        std::cout << "\n";
    }

    std::cout << "trying to create a CategoricalAttribute\n";
    size_t string_column_num = 0;
    size_t double_column_num = 1;
    size_t integer_column_num = 2;
    model::TypedColumnData const& string_column = typed_relation_->GetColumnData(string_column_num);
    model::TypedColumnData const& double_column = typed_relation_->GetColumnData(double_column_num);
    model::TypedColumnData const& int_column = typed_relation_->GetColumnData(integer_column_num);
    std::unique_ptr<Feature> string_feat = CreateFeature(string_column);
    std::unique_ptr<Feature> double_feat = CreateFeature(double_column);
    std::unique_ptr<Feature> int_feat = CreateFeature(int_column);
    std::cout << double_feat->GetTypeId();
}

unsigned long long DES::ExecuteInternal() {
    Test();

    //MAIN FUNCTION OF DESOLVER
    auto time = (long long) 0;

    string s_name = "arm.set";

    srand(1);

    printf("ARM setup: setup_file_name = %s.\n", s_name.c_str());

    // read setup file
    Setup setup(s_name);

    if (!setup.read()) {
        printf("ARM setup: File %s not read.\n", s_name.c_str());
        exit(-1);
    }
    setup.print_param();

    Problem prob;
    string databasename = setup.get_tdbase_name();
    printf("Reading transaction database, f_name= %s...\n", databasename.c_str());
    prob.init_tdbase(setup, databasename);
    Archive rules;
    solve(setup, prob, rules);
    string str2 = setup.get_out_name();
    printf("Writing file, f_name= %s...\n", str2.c_str());
    rules.asort();    // sort the archive
    rules.write(str2, prob);

    return time;
}


} // namespace algos
