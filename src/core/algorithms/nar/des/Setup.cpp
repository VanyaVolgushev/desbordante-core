#include <bits/stdc++.h>
#include "Setup.h"

Setup::Setup(string fname)
{
	// TODO Auto-generated constructor stub
	arm_set_name = fname;
}

Setup::~Setup()
{
	// TODO Auto-generated destructor stub
}

/**
 * Read setup file.
 *
 * @param no input parameters.
 * @return binary return code: =1, if setup file is read successfully, or =0, if input file cannot be found.
 */
bool Setup::read()
{
	ifstream in(arm_set_name);
	string line;
	bool readed = true;

	if (in.is_open()) {
		int block = 0;
		int parse_level = 0;

		while (getline(in, line)) {
			// Vector of string to save tokens
			vector <string> tokens;
			token_line(line, tokens);

			if (line.size() > 0) {	// line cannot be empty!

				if (tokens[0].compare("Problem") == 0) {	// program block
					block = 1;
                } else if (tokens[0].compare("Algorithm") == 0) {// algorithm block
                    block = 2;
                } else if (tokens[0].compare("Visualisation") == 0) {// visualizationblock
                    block = 3;
                } else if (tokens[0].compare("Squash") == 0) {// squashing
                    block = 4;
				} else if (tokens[0][0] == '%') {	// comment line - ignore
					continue;
				} else if (tokens[0][0] == '{') {	// start of block
					parse_level = block;
				} else if (tokens[0][0] == '}') {	// end of block
					parse_level = 0;
				} else {	// parsing blocks
					switch (parse_level) {
					case 1:		// parsing program block
						parse_prob_blk(tokens);
						break;
					case 2:		// parsing algorithm block
						parse_alg_blk(tokens);
						break;
					}
				}

			}
			tokens.clear();
		}
	} else {
		readed = false;
	}

	return readed;
}

/**
 * Tokenize input line stream into tokens.
 *
 * @param the input line, and the vector of parsed tokens.
 * @return no return code.
 */
void Setup::token_line(string line, vector<string> &tokens)
{
	// stringstream class check1
	stringstream check1(line);
	string intermediate;

	// Tokenizing w.r.t. space ' '
	while(getline(check1, intermediate, ' '))
	{
		tokens.push_back(intermediate);
	}
}

/**
 * Parse the problem definition part.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_prob_blk(vector<string>tokens)
{
	if(tokens[0].compare("Tdbase_name") == 0) {
		tdbase_name = tokens[2];
	} else if(tokens[0].compare("Rule_name") == 0) {
		rule_name = tokens[2];
	} else if(tokens[0].compare("Out_name") == 0) {
		out_name = tokens[2];
	}
}

/**
 * Parse the kind of algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_alg_blk(vector<string>tokens)
{
    parse_alg_de(tokens);
}

/**
 * Parse the parameter setting of the DE algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_alg_de(vector<string>tokens)
{
	if (tokens[0].compare("DE_PARAM") == 0) {
		cout << "DE_PARAM started..." << endl;
	} else if (tokens[0].compare("DE_NP") == 0) {
		Np = stoi(tokens[2]);
	} else if (tokens[0].compare("DE_FES") == 0) {
		FEs = stoi(tokens[2]);

	} else if (tokens[0].compare("DE_F") == 0) {
		alg_param.de.scale = stof(tokens[2]);
	} else if (tokens[0].compare("DE_CR") == 0) {
		alg_param.de.xover = stof(tokens[2]);
	} else if (tokens[0].compare("DE_STRATEGY") == 0) {
		alg_param.de.strategy = stoi(tokens[2]);
	}
}

/**
 * Print out all three parameter parts determining a behavior of the solver.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_param()
{
    print_prob_blk();
    print_alg_blk();
}

/**
 * Print out the problem definition part.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_prob_blk()
{
	cout << "Problem block setup: " << endl;
	cout << "---------------------" << endl;
	cout << "Tdbase_name= " << tdbase_name << endl;
	cout << "Rule_name= " << rule_name << endl;
	cout << "Out_name= " << out_name << endl;
	cout << "---------------------" << endl;
}

/**
 * Print out the algorithm's parameter setting.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_alg_blk()
{
	cout << "Optimization block setup: " << endl;
	cout << "---------------------" << endl;

	cout << "Algorithm= DE" << endl;
	cout << "NP= " << Np << endl;
	cout << "FES= " << FEs << endl;
	cout << "F= " << alg_param.de.scale << endl;
	cout << "CR= " << alg_param.de.xover << endl;
	cout << "Strategy= " << alg_param.de.strategy << endl;
	cout << "---------------------" << endl;
}
