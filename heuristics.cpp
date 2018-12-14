/*
 * The user may define in this file the splitting rule
 * a.k.a. the heuristics rule in the David-Putnam Algorithm.
 * A heuristic is a function that, given a SAT instance r
 * represented by a SAT struct in solver.h, return a variable
 * to split search on (as a positive integer).
 * Note that this variable MUST occur at least once in the 
 * formula. 
 * It is the user's responsibility to ensure this. I provide
 * Two examples to use. 
 * You May NOT delete this file!
 *
 */


#include "solver-utils.cpp"

/*
 * Heuristic for choosing the splitting variable
 * return the variable on success, return -1 on failure
 *
 */
var choose_literal(SAT S){
	for(int v = 1; v < S.meta.num_vars + 1; v++){
		if(is_open(S, v)){
			return v;
		}
	}
	return -1;
}