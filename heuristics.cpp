/*
 * The user may define in this file the splitting rule
 * a.k.a. the heuristics rule in the David-Putnam Algorithm.
 * A heuristic is a function that, given a SAT instance r
 * represented by a SAT struct in solver.h, return a variable
 * to split search on (as a positive integer).
 * Note that this variable MUST occur at least once in the 
 * formula. 
 * It is advised that if no choice of splitting variable is 
 * available or the function runs into error, return -1. 
 * It is the user's responsibility to ensure this. I provide
 * Two examples to use. 
 * You May NOT delete this file!
 *
 */


#include "solver-utils.cpp"


/*
 * Heuristic for choosing the splitting variable
 * return the variable on success, return -1 on failure
 * Simple Heuristic: returns the first variable that appears.
 *
 */

/*
var choose_literal(SAT S){
	for(int v = 1; v < S.meta.num_vars + 1; v++){
		if(is_open(S, v)){
			return v;
		}
	}
	return -1;
}
*/

/*
 * Heuristic for choosing the splitting variable
 * return the variable on success, return -1 on failure
 * A slightly more sophisticated 
 * heuristic: returns the variable that occurs most often,
 * counting both itself and its negation. 
 * As one might expect, this heuristic performs better 
 * than the naive one above. 
 *
 */

var choose_literal(SAT S){
	int max = 0;
	var curr = -1;
	for(int v = 1; v < S.meta.num_vars + 1; v++){
		if(is_open(S, v)){
			int size = S.literal_indices[v-1].size() + 
				S.neg_indices[v-1].size();
			if(size > max){
				curr = v;
				max = size;
			}
		}
	}
	return curr;
}






