

#include "solver.h"

int main(){

	SAT S = SAT();
	metadata M = metadata();
	M.num_vars = 100;
	M.num_clauses = 1;
	M.type_prob = "CNF";
	S.meta = M;
	cout << S.meta.num_vars; 
	cout << S.meta.num_clauses;
	return 0;
}