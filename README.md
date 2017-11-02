# SAT-solver
homework for LVR

the source code is homework.cpp (written in C++)

instructions for running (command-line command): homework.exe 'inputfilename.txt' 'outputfilename.txt' (for e.g. homework.exe test01.txt output.txt)

the chosen example CNF formula is sudoku_myexample02.txt


some comments:

the algohithm used is (complete) DPLL with a heuristic, namely, the algorithm chooses the literal l, with which we make an assumption, to be the most frequently occuring literal in the formula currently inspected.

test03.txt is an example (found online) with 60 variables and 160 clauses that the algorithm fails to solve (in a reasonable time)

sudoku_myexample01.txt is the formula corresponding to the sudoku in myexample.bmp

sudoku_myexample02.txt is a formula corresponding to the sudoku in myexample.bmp with only the first 20 numbers entered (counting up to down, left to right), which is solved in about 10sec

sudoku_myexample03.txt is the same as the previous but with the first 19 numbers entered (this takes about 1 min 20 sec to solve)
