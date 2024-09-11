#pragma once

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

    using namespace std;

    // algorithm's setups
    struct de_param {
        double scale;
        double xover;
        int strategy;
    };

/**
 * Definition of setup parameter input file
 *
 * The class is defined for manipulation of input parameters saved
 * with which a behavior of the solver is controlled. These parameters
 * are saved into an input file that is divided into three parts:
 * problem definition, parameter setting of algorithm dedicated for
 * ARM, and parameter setting of visualization method dedicated for
 * preparing data for visualization. The third part is not implemented
 * yet.
 *
 */
    class Setup {
    public:
        Setup(string fname);

        virtual ~Setup();

        bool read();

        void token_line(string line, vector<string> &tokens);

        void parse_prob_blk(vector<string> tokens);

        void parse_alg_blk(vector<string> tokens);

        void parse_alg_de(vector<string> tokens);

        void print_prob_blk();

        void print_alg_blk();

        void print_param();

        // getters
        int get_Np() { return Np; }

        int get_FEs() { return FEs; }

        string get_tdbase_name() { return tdbase_name; }

        string get_rule_name() { return rule_name; }

        string get_out_name() { return out_name; }

        // setters
        void set_Np(int val) { Np = val; }

        void set_FEs(int val) { FEs = val; }

        void set_tdbase_name(string str) { tdbase_name = str; }

        void set_rule_name(string str) { rule_name = str; }

        void set_out_name(string str) { out_name = str; }

    private:
        int Np;                ///< population size
        int FEs;            ///< number of fitness function evaluations

        string arm_set_name;    ///< ARM setup file name
        string tdbase_name;    ///< transaction database
        string rule_name;        ///< rule database
        string out_name;        ///< output file name

    public:
        union {
            de_param de;
        } alg_param;            ///< algorithm parameters
    };
