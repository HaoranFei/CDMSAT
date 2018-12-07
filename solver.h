/*
 * A light-weight DPLL SAT solver
 * By Haoran Fei, andrew ID: hfei
 * For 15-354 Final Project, Fall 2018
 * Implemented in C++
 *
*/


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <boost/python.hpp>

using namespace boost::python;



struct metadata{
	size_t num_vars;
	size_t num_clauses;
	//Should be "cnf"
	char* type_prob;
};

typedef struct metadata* metadata;
//According to DiMAC SAT specification, each variable is a int
//<0: negation
//>0: variable (1 ~ n)
//0: termination cahracter for each clause
typedef int var;

struct single_clause{
	size_t clause_length;
	var* vars;
};

typedef struct single_clause* clause;


struct SAT_instance{
	metadata meta;
	size_t* literal_indices;
	size_t* neg_indices;
	clause* clauses;
};

typedef struct SAT_instance* SAT;

bool is_neg(var v);
bool is_unit_clause(clause C);
bool is_pure(SAT S, var v);






