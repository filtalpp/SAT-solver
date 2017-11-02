#include <iostream>
#include <fstream>
#include <string.h>
#include <string>

using namespace std;

const short TRUE = 30000, FALSE = 0;
short *result, nvar, resultc = 0;
int *varcount;
ifstream infile;
ofstream fout;
	
struct cnf_formula
{
	int num_clauses;
	short *cllen;
	short **f;
};

void print_formula(cnf_formula A)
{
	for(int i=0; i < A.num_clauses; i++)
	{
		for(int j=0; j < A.cllen[i]; j++)
			fout << A.f[i][j] << ' ';
		fout << endl;
	}
	fout << endl;
}

void read_input(cnf_formula &A)
{
	int i, j, p;
	short I[30000];

	string line;
	char a;

	// we read the input
	infile >> a;
	while(a == 'c')
	{
		getline(infile, line);
		infile >> a;
	}
	for(i=0; i < 3; i++)		
		infile >> a;
		
	infile >> nvar;
	infile >> A.num_clauses;

	A.cllen = new short[A.num_clauses];
	A.f = new short*[A.num_clauses];
	for(i = 0; i < A.num_clauses; i++)
	{
		j = 0;
		infile >> I[j];
		while(I[j] != 0)
			infile >> I[++j];

		A.f[i] = new short [j];
		for(p = 0; p < j; p++)
			A.f[i][p] = I[p];
		 
		A.cllen[i] = j;
	}

	result = new short[nvar+3];
	varcount = new int[nvar+3];
	infile.close();
}

void _delete(cnf_formula input)
{
	if(input.f != 0) 
	{
		for(int j = 0; j < input.num_clauses; j++)
			if(input.f[j] != 0) delete [] input.f[j];
		delete [] input.f;
	}
	if(input.cllen != 0) delete [] input.cllen;	
}

// on each call of solve(): if we don't find a unit clause, we make a copy of (part of) the whole formula
// ucl is a auxiliary variable which is true iff we have called solve() knowing that the first clause of input is a unit clause
bool solve(cnf_formula input)
{
	int i, j, k, t = 0, unit_clause = -1, max;
	short lit;
	char found;
	cnf_formula newformula;
	
	// first print input (the input formula)
	//print_formula(input);

	// if we have no clauses (so the formula is satisfiable)
	if(input.num_clauses == 0)
	{
		for(i=0; i < resultc; i++)
			fout << result[i] << ' ';

	    return 1;
	}

	// we search for an empty clause (so the formula is unsatisfiable) or a unit clause
	for(i = 0; i < input.num_clauses; i++)
		if(input.cllen[i] == 0) 
			return 0;
		else if(input.cllen[i] == 1 & unit_clause == -1) 
			unit_clause = i;

	// if there is a unit clause
	if(unit_clause != -1)		// we found a unit clause
	{
		i = unit_clause;
		newformula.f = new short*[input.num_clauses+1];
		newformula.cllen = new short[input.num_clauses+1];
		newformula.num_clauses = 0;
	
		for(j = 0; j < input.num_clauses; j++)		// we copy the resulting simplified formula to newformula, and afterwards we delete input
		{
			newformula.f[newformula.num_clauses] = new short[input.cllen[j]];
			newformula.cllen[newformula.num_clauses] = input.cllen[j];
				
			t = 0;
			for(k = 0; k < input.cllen[j]; k++)		// we copy the j-th clause of input
			{
				newformula.f[newformula.num_clauses][k-t] = input.f[j][k];
				if(input.f[j][k] == input.f[i][0]) 
				{ 
					newformula.num_clauses--;
					break;
				}
				else if(input.f[j][k] == -input.f[i][0])
				{
					newformula.cllen[newformula.num_clauses]--;
					t = 1;
				}
			}
			newformula.num_clauses++;	
		}

		result[resultc++] = input.f[i][0];
		// we delete input
		_delete(input);
		
		// try to solve the simplified formula
		t = solve(newformula);
		result[--resultc] = 0;
		
		return t;
	}
	
	// no unit clause was found. now we make an assumption.
	
	// first we search for the most frequently occuring variable in input
	for(i = 1; i < nvar+3; i++)
		varcount[i] = 0;
	max = 0;

	for(i = 0; i < input.num_clauses; i++)
		for(j = 0; j < input.cllen[i]; j++)
			if(input.f[i][j] < 0)
			{
				++varcount[-input.f[i][j]];
				if(varcount[-input.f[i][j]] > max)
				{
					max = varcount[-input.f[i][j]];
					lit = input.f[i][j];
				}
			}
			else
			{
				++varcount[input.f[i][j]];
				if(varcount[input.f[i][j]] > max)
				{
					max = varcount[input.f[i][j]];
					lit = input.f[i][j];
				}
			}
			
	// now lit is the literal that we are going to assume to be true and to be false
		
	// now we simplify the input formula (and make a new formula newformula. we don't change input)
		
	// we try solving with assumption lit, 
	newformula.f = new short*[input.num_clauses+1];
	newformula.cllen = new short[input.num_clauses+1];
	newformula.num_clauses = 0;
	
	for(j = 0; j < input.num_clauses; j++)		// we copy the resulting simplified formula to newformula
	{
		newformula.f[newformula.num_clauses] = new short[input.cllen[j]];
		newformula.cllen[newformula.num_clauses] = input.cllen[j];
				
		t = 0;
		for(k = 0; k < input.cllen[j]; k++)		// we copy the j-th clause of input
		{
			newformula.f[newformula.num_clauses][k-t] = input.f[j][k];
			if(input.f[j][k] == lit) 
			{ 
				newformula.num_clauses--;
				break;
			}
			else if(input.f[j][k] == -lit)
			{
				newformula.cllen[newformula.num_clauses]--;
				t = 1;
			}
		}
		newformula.num_clauses++;	
	}
	
	result[resultc++] = lit;	
	t = solve(newformula);
	result[--resultc] = 0;
	if(t == 1) return 1;

	// we try solving with assumption -lit
	lit = -lit;
	newformula.f = new short*[input.num_clauses+1];
	newformula.cllen = new short[input.num_clauses+1];
	newformula.num_clauses = 0;
	
	for(j = 0; j < input.num_clauses; j++)		// we copy the resulting simplified formula to newformula
	{
		newformula.f[newformula.num_clauses] = new short[input.cllen[j]];
		newformula.cllen[newformula.num_clauses] = input.cllen[j];
				
		t = 0;
		for(k = 0; k < input.cllen[j]; k++)		// we copy the j-th clause of input
		{
			newformula.f[newformula.num_clauses][k-t] = input.f[j][k];
			if(input.f[j][k] == lit) 
			{ 
				newformula.num_clauses--;
				break;
			}
			else if(input.f[j][k] == -lit)
			{
				newformula.cllen[newformula.num_clauses]--;
				t = 1;
			}
		}
		newformula.num_clauses++;	
	}
	
	result[resultc++] = lit;	
	t = solve(newformula);
	result[--resultc] = 0;
	if(t == 1) return 1;


	_delete(input);

	return 0;
}

int main(int argc, char *argv[])
{
	if(argc < 3) return 0;

	cnf_formula A;
	
	infile.open(argv[1]);
	fout.open(argv[2]);
	
	read_input(A);
	
	if(!solve(A))
		fout << 0;
	
	delete [] result;
	delete [] varcount;
	fout.close();
	
	return 0;
}
