
#include "heuristics.cpp"

/*
 * Perform DPLL unit propogation for every unit clause in SAT instance S
 * 
 *
*/

SAT unit_propagate(SAT S){
	vector<tuple<var, bool>> to_assign;
	SAT output = copy_SAT(S);
	for(int i = 0; i < S.clauses.size(); i ++){
		if(is_unit_clause(S.clauses[i])){
			//There is only one variable!
			var unit_var = S.clauses[i].vars[0];
			if(unit_var > 0){
				to_assign.push_back(make_tuple(unit_var, true));
			}
			else{
				to_assign.push_back(make_tuple(abs(unit_var), false));
			}
		}
	}
	for(int j = 0; j < to_assign.size(); j ++){
		output = assign(output, get<0>(to_assign[j]), get<1>(to_assign[j]));
	}
	return output;
}

/*
 * Requires: v is a pure variable in l
 * Eliminates all clauses containing v by assigning them with 
 * appropriate value
 *
 */
SAT pure_literal_assign(SAT S){
	SAT output = copy_SAT(S);
	for(int v = 1; v < S.meta.num_vars + 1; v++){
		//cout << "v is: " << v << "\n";
		//cout << "is_pure(S, 3)" << is_pure(S, 3) << "\n";
		//output = assign(output, 1, true);
		//output = assign(output, 2, true);
		
		if(is_pure(S, v)){
			//All occurances are positive
			if(!S.literal_indices[v-1].empty()){
				output = assign(output, v, true);
			}
			//All occurances are negative
			else{
				output = assign(output, v, false);
			}
		}
	}
	return output;
}


/* 
 * Try to assign a variable with a truth value
 * Normally return a clause, now assigned with said truth values
 * If clause does not contain the specified variable, then a copy
 * is returned.
 * If the clause,after assignment, is the constant true clause,
 * Then return a special new clause with clause_length -1
 * 
 * If the clause, after assignment, is the constant false clause, 
 * Then return a special new clause with clause_length = 0
 * Which is empty clause.
 *
*/

clause assign_clause(clause l, var v, bool value){
	clause output = copy_clause(l);
	ulong i = 0;
	//cout <<"output.clause_length is: " << output.clause_length << "\n";

	if(output.clause_length == -1){
		//Constant true clause
		return output;
	}
	if(output.clause_length == 0){
		//Constant false clause
		return output;
	}

	while(i < output.vars.size()){
		if(abs(output.vars[i]) == v){

			//negation, assign to true
			//literal = false
			//delete from formula
			if(output.vars[i] < 0 && value){
				output.vars.erase(output.vars.begin() + i);
				output.clause_length -= 1;
				//index remains unchanged
			}
			//Assign to true
			//literal = true
			else if(output.vars[i] > 0 && value){
				//cout << "Found constant true clause! \n";
				output.clause_length = -1;
				return output;
			}
			//negation, assign to false
			//literal = true
			else if(output.vars[i] < 0 && !value){
				output.clause_length = -1;
				return output;
			}
			//Assign to false
			//literal = false
			//delete from formula
			else{
				output.vars.erase(output.vars.begin() + i);
				output.clause_length -= 1;
				//index remains unchange
			}
		}
		else{
			i += 1;
		}
	}
	return output;
}


SAT assign(SAT S, var v, bool value){
	SAT output = copy_SAT(S);

	//If SAT instance is already determined, then no need to assign
	//Just return
	if(output.meta.is_constant_true){
		return output;
	}
	else if(output.meta.is_constant_false){
		return output;
	}

	//cout << "\noutput.meta.num_clauses = " << output.meta.num_clauses << "\n";

	int open_clause_counter = 0;
	for(int i = 0; i < output.meta.num_clauses; i++){
		//cout << "This is right before assigning the ith clause, i = " << i << "\n";
		clause newclause = assign_clause(output.clauses[i], v, value);
		//A clause evaluates to constant true
		//Update the reference list of variables
		if(newclause.clause_length == -1){
			for(int j = 0; j < output.clauses[i].vars.size(); j++){
				//cout << "j is: " << j << "\n";

				var curr_var = output.clauses[i].vars[j];
				vector<ulong>::iterator it;

				//cout << "curr_var is: " << curr_var << "\n";

				//Eliminate current clause from positive index list
				if(curr_var > 0){
					it = find(output.literal_indices[curr_var -1].begin(), 
						output.literal_indices[curr_var -1].end(), i);
					if(it != output.literal_indices[curr_var -1].end()){
						output.literal_indices[curr_var -1].erase(it);
					}
				}
				//Eliminate current clause from negative index list;
				else if (curr_var < 0){
					it = find(output.neg_indices[abs(curr_var) -1].begin(), 
						output.neg_indices[abs(curr_var) -1].end(), i);
					//cout << "get past find \n";
					if(it != output.neg_indices[abs(curr_var) -1].end()){
						output.neg_indices[abs(curr_var) -1].erase(it);
					}
					//cout << "get past erase \n";
				}
			}
			output.clauses[i] = newclause;
		}

		//We have an empty clause
		//The SAT instance is not satisfiable!!
		else if(newclause.clause_length == 0){
			output.meta.is_constant_false = true;
			break;
		}
		//Normal clause
		else{
			//Erase variable to assign from index lists
			//SHOULD NOT USE NEWCLUASE
			//BUG MUST BE FIXED TOMORROW
			for(int j = 0; j < output.clauses[i].vars.size(); j++){
				var curr_var = output.clauses[i].vars[j];
				vector<ulong>::iterator it;
				var curr_var_abs = abs(curr_var);

				//Eliminate current variable from positive index list
				if(curr_var_abs == v && curr_var > 0){
					it = find(output.literal_indices[curr_var -1].begin(), 
						output.literal_indices[curr_var -1].end(), i);
					output.literal_indices[curr_var -1].erase(it);
				}
				//Eliminate current variable from negative index list;
				else if (curr_var_abs == v && curr_var < 0){
					it = find(output.neg_indices[abs(curr_var) -1].begin(), 
						output.neg_indices[abs(curr_var) -1].end(), i);
					output.neg_indices[abs(curr_var) -1].erase(it);
				}
			}

			output.clauses[i] = newclause;
			open_clause_counter += 1;
		}
	}
	output.meta.num_open_clauses = open_clause_counter;

	//If the assigned SAT instance has no more open clause
	//And that it is not constant false, then it is true
	if(open_clause_counter == 0){
		if(!output.meta.is_constant_false){
			output.meta.is_constant_true = true;
		}
	}
	return output;
}


bool DPLL_solve(SAT S){
	if(S.meta.is_constant_true){
		return true;
	}
	else if(S.meta.is_constant_false){
		return false;
	}
	else{
		SAT output = copy_SAT(S);
		output = unit_propagate(output);
		output = pure_literal_assign(output);
		//After the processing, check if S is solved
		if(output.meta.is_constant_true){
			return true;
		}
		else if(output.meta.is_constant_false){
			return false;
		}
		else{
			var chosen = choose_literal(output);
			return DPLL_solve(assign(output, chosen, true)) 
			    || DPLL_solve(assign(output, chosen, false));
		}

	}
}
