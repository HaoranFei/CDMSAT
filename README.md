# CDMSAT
A Lightweight 3SAT solver implementation based on DPLL algorithm. For final project of 15-354 Computational Discrete Math, Fall 2018, Carnegie Mellon University. 
Source code in C++, using Boost. 

# Dependency
C++ 11 <br />
g++ compiler <br />
Latest version of Boost <br />
The project has only been tested on Linux platforms. It might not work on other Operating Systems. Be careful. 

# Usage
To compile the executable, on a Bash terminal, run <br />
$ make 

To run the compiled executable, do <br />
$ ./solver.out ./tests/<name_of_test_file> 

To recompile, do <br />
$ make clean <br />
$ make

# Heuristics Selection
It is common knowledge that the choice of the splitting variable, often called the
splitting rule or the splitting heuristic for DPLL algorithms, is vital to 
improving performance in real tests. <br />
In fact, some cases require exponential time
for one splitting strategy while low-polynomial time for other strategies. <br />
Therefore, the user of this module may design and implement their own splitting
heuristics in a dedicated file, heuristics.cpp. This file should ONLY contain the
splitting heuristics function. <br />
The header comment of this file contains more information on how to implement such
a strategy. It also includes two examples. <br />

# Testing
The tests directory contains a couple of test files
Test files are in standard DIMACS CNF-SAT format.<br />
The program will not accept any other format of input file. <br />
If you want to use your own test file, notice that some DIMACS files end with a 0
delimiter after final clause. <br />
Please delete it, or program will not run. <br />
If you want to find more test files, go to this website: <br />
http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html

# Performance
Takes about 5-10s (depend on machine) on mid3-80.txt, which contains 50 variables 
and 80 clauses. <br />
If you test on a much larger input, expect delays. <br />
There is a hard cap on number of input clauses, as macro MAX_NUM_CLAUSES. Currently 
set to 100000. Can be adjusted. 
