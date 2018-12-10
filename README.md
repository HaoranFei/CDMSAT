# CDMSAT
A Lightweight 3SAT solver implementation based on DPLL algorithm. For final project of 15-354 Computational Discrete Math, Fall 2018, Carnegie Mellon University. 
Source code in C++, using Boost. 

# Dependency
C++ 11
g++ compiler
Latest version of Boost 

# Usage
To compile the executable, on a Bash terminal, run
$ make 

To run the compiled executable, do 
$ ./solver.out ./tests/<name_of_test_file>

To recompile, do
$ make clean
$ make

# Testing
The tests directory contains a couple of test files
Test files are in standard DIMACS CNF-SAT format.
The program will not accept any other format of input file
If you want to use your own test file, notice that some DIMACS files end with a 0
delimiter after final clause. Please delete it, or program will not run. 
If you want to find more test files, go to this website:
http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html

# Performance
Takes about 5-10s (depend on machine) on mid3-80.txt, which contains 50 variables 
and 80 clauses.
If you test on a much larger input, expect delays. 
There is a hard cap on number of input clauses, as macro MAX_NUM_CLAUSES. Currently 
set to 100000. Can be adjusted. 