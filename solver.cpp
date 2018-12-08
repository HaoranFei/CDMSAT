

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

	bool res = DPLL_solve(original);
	print_res(res);

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

	/*
	for(int i = 0; i < original.meta.num_vars; i++){
		print_vector(original.literal_indices[i]);
		print_vector(original.neg_indices[i]);
	}
	*/
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
bool is_pure(SAT S, var v);

SAT unit_propagate(clause l, SAT S);

SAT pure_literal_assign(clause l, var v);

var choose_literal(SAT S);

clause assign_clause(clause l, var v, bool value);

SAT assign(SAT S, var v, bool value);


bool DPLL_solve(SAT S){
	return true;
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
