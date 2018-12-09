

#include "solver.h"


int main(int argc, char *argv[]){
	if(argc != 2){
		cout << "Usage: ./dpll <test_file_name> \n";
		return 0;
	}
	string infile_name = argv[1];
	fstream infile;
	infile.open(infile_name);

	if(infile.is_open() == false){
		cout << "Sorry. Fail to open specified file \n";
		return 0;
	}

	metadata M = metadata();
	SAT original = SAT();
	M.is_constant_true = false;
	M.is_constant_false = false;
	bool found_problem_line = false;
	string line;

	ulong start_of_formula_pt = 0;



	while(getline(infile, line)){
		//Parse the input file before the problem line (There should only be 1)

		string instruction = line.substr(0, line.find(" "));
		if(instruction.compare("c") == 0){
			continue;
		}
		else if(instruction.compare("p") == 0){
			found_problem_line = true;
			vector<string> parsed_problem_line;
			boost::algorithm::split(parsed_problem_line, line, 
				boost::algorithm::is_any_of(" "));

			if(parsed_problem_line[0] != "p"){
				cout << "The problem line does not start with p. Exit \n";
				infile.close();
				return 0;
			}
			else if(parsed_problem_line[1] != "cnf"){
				cout << "The input instance is not in CNF. Exit \n";
				infile.close();
				return 0;
			}

			//Fill the matadata of the original formula

			ulong num_vars, num_clauses;
			num_vars = (ulong) stoi(parsed_problem_line[2]);
			num_clauses = (ulong) stoi(parsed_problem_line[3]);

			//Checks for inputs that are too large
			if(num_clauses > MAX_NUM_CLAUSES){
				cout << "The input clause number is too large: " << num_clauses << "\n";
				infile.close();
				return 0;
			}

			M.num_vars = num_vars;
			M.num_clauses = num_clauses;
			M.num_open_clauses = num_clauses;
			M.type_prob = "cnf";
			original.meta = M;
			
			//Construct the index list for the SAT instance
			//Each sublist is initially empty
			vector<vector<ulong>> literal_indices;
			vector<vector<ulong>> neg_indices;

			for(int i = 0; i < num_vars; i ++){
				vector<ulong> new_vector1;
				vector<ulong> new_vector2;
				literal_indices.push_back(new_vector1);
				neg_indices.push_back(new_vector2);
			}

			original.literal_indices = literal_indices;
			original.neg_indices = neg_indices;

			start_of_formula_pt = infile.tellg();
			break;

		}
		else{
			cout << "Found malformed line before problem line. Exit \n";
			infile.close();
			return 0;
		}
	}

	//Tries to parse out the rest of infile as a single string
	stringstream buffer;
	fstream temp;
	temp.open(infile_name);
	buffer << temp.rdbuf();
	temp.close();

	string complete_formula = buffer.str();
	complete_formula = complete_formula.substr(start_of_formula_pt, string::npos);

	//Replace all occurances of standalone 0s in the string (i.e. delimiter)
	//With a character that cannot occur in a normal number
	//For ease of parsing later
	boost::replace_all(complete_formula, " 0", " #");

	vector<string> formula_strings;
	boost::algorithm::split(formula_strings, complete_formula, 
				boost::algorithm::is_any_of("#"));



	//Splits the formulas according to DIMACS spec
	//Using 0 as delimiter
	//Checks that number of clauses is consistent
	if(formula_strings.size() != M.num_clauses){
		cout << "The number of clauses scanned does not match amount in problem line. Exit";
		infile.close();
		return 0;
	}

	for(int i = 0; i < formula_strings.size(); i ++){

		string formula = formula_strings[i];
		stringstream ss;
		ss.str(formula);

		//create new clause
		clause l = clause();

		var number;

		std::vector<var> scanned_vars;
		while (ss >> number){
			
			//Adding the current clause to the appropriate clause list
			//primarily used for pure-clause elimination
			int var_index = abs(number);
			if(polarity(number)){
				original.literal_indices[var_index-1].push_back(i);
			}
			else{
				original.neg_indices[var_index-1].push_back(i);
			}

			//Added the scanned variable to variable list of clause
  			scanned_vars.push_back(number);
		}
		l.vars = scanned_vars;
		l.clause_length = scanned_vars.size();
		original.clauses.push_back(l);
	}


	//bool test1 = is_pure(original, 3);
	//cout << "Variable 3 is pure, then print 1, else print 0: " << test1 << "\n"; 

	//original.clauses[1] = assign_clause(original.clauses[1], 4, false);
	//cout << "Length of first clause is: " << original.clauses[1].clause_length << "\n";

	SAT S2 = assign(original, 4, true);

	//bool res = DPLL_solve(original);

	//print_res(res);

	/*
	for(int i = 0; i < original.meta.num_clauses; i++){
		clause C = original.clauses[i];
		cout << "This is clause No: " << i << "\n";
		for(int j = 0; j < C.clause_length; j++){
			cout << "This is the " << j << "th element: " << C.vars[j] << "\n";
		}
	}
	return 0;
	*/

	
	for(int i = 0; i < S2.meta.num_vars; i++){
		cout << "Now printing lists for variable: " << i+1 << "\n";
		print_vector(S2.literal_indices[i]);
		print_vector(S2.neg_indices[i]);
		cout << "\n";
	}
	
}

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



SAT unit_propagate(clause l, SAT S);

SAT pure_literal_assign(clause l, var v);

var choose_literal(SAT S);

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

	//cout << "output.meta.num_clauses = " << output.meta.num_clauses << "\n";

	int open_clause_counter = 0;
	for(int i = 0; i < output.meta.num_clauses; i++){
		clause newclause = assign_clause(output.clauses[i], v, value);
		//A clause evaluates to constant true
		//Update the reference list of variables
		if(newclause.clause_length == -1){
			for(int j = 0; j < output.clauses[i].vars.size(); j++){
				var curr_var = output.clauses[i].vars[j];
				vector<ulong>::iterator it;

				//Eliminate current clause from positive index list
				if(curr_var > 0){
					it = find(output.literal_indices[curr_var -1].begin(), 
						output.literal_indices[curr_var -1].end(), i);
					output.literal_indices[curr_var -1].erase(it);
				}
				//Eliminate current clause from negative index list;
				else if (curr_var < 0){
					it = find(output.neg_indices[abs(curr_var) -1].begin(), 
						output.neg_indices[abs(curr_var) -1].end(), i);
					output.neg_indices[abs(curr_var) -1].erase(it);
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

	}
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
