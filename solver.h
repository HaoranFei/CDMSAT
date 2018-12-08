/*
 *
 * A light-weight DPLL SAT solver
 * By Haoran Fei, andrew ID: hfei
 * For 15-354 Final Project, Fall 2018
 * Implemented in C++
 *
*/


#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <boost/algorithm/string.hpp>
#define MAX_NUM_CLAUSES 100000

using namespace std;


struct metadata{
	size_t num_vars;
	size_t num_clauses;
	//Should be "cnf"
	string type_prob;
};

//According to DiMAC SAT specification, each variable is a int
//<0: negation
//>0: variable (1 ~ n)
//0: termination character for each clause
typedef int var;
typedef unsigned long ulong;

struct clause{
	ulong clause_length;
	vector<var> vars;
};

//typedef struct single_clause* clause;


struct SAT{
	/*
	 * A struct to keep track of the current state of 
	 * This SAT instance
	 */
	metadata meta;
	/* 
	 * For each variable, we keep track of a list of indices
	 * of clauses, where this variable appears
	 */
	vector<vector<ulong>> literal_indices;
	/* 
	 * For each variable, we also keep track of a list of indices
	 * of clauses, where negation of this variable appears
	 */
	vector<vector<ulong>> neg_indices;
	/*
	 * A resizable array of clauses
	 */
	vector<clause> clauses;
};

//typedef struct SAT_instance* SAT;

bool polarity(var v);
bool is_unit_clause(clause C);
bool is_empty_clause(clause C);

bool is_pure(SAT S, var v);

SAT unit_propagate(clause l, SAT S);

SAT pure_literal_assign(clause l, var v);

var choose_literal(SAT S);

clause assign_clause(clause l, var v, bool value);

SAT assign(SAT S, var v, bool value);

bool DPLL_solve(SAT S);

void print_res(bool res);

void print_vector(vector<ulong> v);








