#include "solver.h"


/*
 * requires that v is not a delimiter: i.e. v != 0
 *
 */
bool polarity(var v){
	if(v > 0){
		return true;
	}
	else{
		return false;
	}
}

/*
 * Returns whether a given clause is unit_clause.
 * I.E. clause that contains only one literal 
 *
 */
bool is_unit_clause(clause C){
	if(C.clause_length == 1){
		return true;
	}
	else{
		return false;
	}
}

/*
 * Returns whether a given clause is empty clause.
 * I.E. clause that contains no free literal 
 */
bool is_empty_clause(clause C){
	if(C.clause_length == 0){
		return true;
	}
	else{
		return false;
	}
}


/*
 * Returns whether a variable var (var > 0) is a pure variable
 * in the SAT formula S. 
 *
 */
bool is_pure(SAT S, var v){
	bool positive_occurs = !S.literal_indices[v-1].empty();
	bool negative_occurs = !S.neg_indices[v-1].empty();
	
	//Explanation: If both are empty, then the variable in not in the formula
	//If one of them is empty, then the variable is pure
	//If neither is empty, then it is not pure
	return positive_occurs != negative_occurs;
}

/*
 * Returns whether a variable var (var > 0) is an open variable
 * in the SAT formula S. 
 *
 */
bool is_open(SAT S, var v){
	bool positive_occurs = !S.literal_indices[v-1].empty();
	bool negative_occurs = !S.neg_indices[v-1].empty();
	
	//Explanation: If both are empty, then the variable in not in the formula
	//If one of them is empty, then the variable is pure
	//If neither is empty, then it is not pure
	return positive_occurs || negative_occurs;
}



/*
 * Creating a new clause that is clone of old clause
 * Deep copy
 */
clause copy_clause(const clause l){
	clause output = clause();
	output.clause_length = l.clause_length;
	vector<var> ovars;
	output.vars = ovars;
	for(int i = 0; i < l.vars.size(); i++){
		output.vars.push_back(l.vars[i]);
	}
	return output;
}

/*
 * Creating a new SAT instance that is clone of old instance
 * Deep copy
 */
SAT copy_SAT(const SAT S){
	SAT output = SAT();
	output.meta = S.meta;
	vector<vector<ulong>> new_pos_list;
	vector<vector<ulong>> new_neg_list;
	output.literal_indices = new_pos_list;
	output.neg_indices = new_neg_list;

	for(int i = 0; i < S.literal_indices.size(); i++){
		vector<ulong> list;
		for(int j = 0; j < S.literal_indices[i].size(); j++){
			list.push_back(S.literal_indices[i][j]);
		}
		output.literal_indices.push_back(list);
	}

	for(int i = 0; i < S.neg_indices.size(); i++){
		vector<ulong> list2;
		for(int j = 0; j < S.neg_indices[i].size(); j++){
			list2.push_back(S.neg_indices[i][j]);
		}
		output.neg_indices.push_back(list2);
	}

	vector<clause> C;
	output.clauses = C;
	for(int k = 0; k < S.clauses.size(); k++){
		output.clauses.push_back(copy_clause(S.clauses[k]));
	}
	return output;
}


void print_res(bool res){
	cout << "This is the satisfiability result of the given instance ";
	cout << "using DPLL algorithm. \nBelow, 1 means satisfiable ";
	cout << "and 0 means not satisfiable. \nThe result is: ";
	cout << res << "\n";
	return;
}


/*
 * Function for debugging purposes solely
 */
void print_vector(vector<ulong> v){
	int length = v.size();
	cout << "Now printing vector: ";
	for(int i = 0; i < length; i++){
		cout << v[i] << " ";
	}
	cout << "\n";
	return;
}
