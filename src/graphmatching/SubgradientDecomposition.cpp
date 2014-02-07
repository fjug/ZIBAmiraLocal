#include <stdio.h>
#include <string.h>
#include "SubgradientDecomposition.h"
#include <hxcore/HxMessage.h>
#include <hxcore/HxWorkArea.h>

#ifdef _OPENMP
#include <omp.h>
#endif

inline double GetNorm2(double* g, int var_num)
{
	int i;
	double norm2 = 0;
	for (i=0; i<var_num; i++) norm2 += g[i]*g[i];
	return norm2;
}

inline void Add(double* vars, double* g, double lambda, int var_num)
{
	double* vars_end = vars + var_num;
	for ( ; vars<vars_end; vars++, g++) *vars += lambda*(*g);
}

SubgradientDecomposition::SubgradientDecomposition(int _n0, double* _x0, void* _user_arg)
	: n0(_n0), x0(_x0), n(0), user_arg(_user_arg)
{
	subproblems = new Block<Subproblem>(128);
	buf = new double[2*n0];
}

SubgradientDecomposition::~SubgradientDecomposition()
{
	delete subproblems;
	delete [] buf;
}

void SubgradientDecomposition::AddSubproblem(Subproblem& _s)
{
	Subproblem* s = subproblems->New();
	memcpy(s, &_s, sizeof(Subproblem));
	n += s->var_num;
}

void SubgradientDecomposition::ReadBest(double* vars)
{
	Subproblem* s;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		memcpy(vars, s->vars, s->var_num*sizeof(double));
		vars += s->var_num;
	}
}

void SubgradientDecomposition::WriteBest(double* vars)
{
	Subproblem* s;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		memcpy(s->vars, vars, s->var_num*sizeof(double));
		vars += s->var_num;
	}
}

double SubgradientDecomposition::ProjectAndComputeSubgradient(double* vars, double* g, DummyFn F_dummy)
{
	F_dummy(user_arg);

	int k = 0;
	double v = 0;
	//Subproblem* s;
	memcpy(buf+n0, x0, n0*sizeof(double));

	int numSubproblems =0;

	for (Subproblem* s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		if (s->weight <= 0) continue;
		for (k=0; k<s->var_num; k++)
		{
			buf[n0+s->array[k]] -= s->weight*(*vars++);
		}

		numSubproblems++;
	}
	vars -= n;

	McDArray<SubgradientDecomposition::Subproblem*> sArray(numSubproblems);
	McDArray<int> subproblemOffset(numSubproblems+1);
	subproblemOffset[0]=0;
	int sIdx=0;
	for (Subproblem* s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		sArray[sIdx]=s;
		subproblemOffset[sIdx+1]=subproblemOffset[sIdx]+s->var_num;
		sIdx++;
	}

	#pragma omp parallel for reduction(+ : v) 
	for (int sIdx=0; sIdx<sArray.size(); sIdx++) 
	{
		Subproblem* s=sArray[sIdx];
		if (s->weight > 0) {
			double* varsS = vars + subproblemOffset[sIdx];
			double* gS = g + subproblemOffset[sIdx];
			for (int k=0; k<s->var_num; k++) varsS[k] += s->weight*buf[n0+s->array[k]]/buf[s->array[k]];
				//ToDo: find out what goes wrong when run in parallel. must be something shared/modified in user_arg.
			double dummy = s->F(*s, varsS, gS, user_arg);
			v += dummy;
			//vars += s->var_num;
			//g += s->var_num;
		}
	}
	return v;
}

double SubgradientDecomposition::Maximize(int iter_max, double gap_threshold, UpperBoundFn F_upper, DummyFn F_dummy)
{
	double theta, z, delta;
	double upper_bound_best = 1e100;
	Subproblem* s;
	int k;
	int gamma; // number of steps since the last improvement of the best objective value
	int gamma_bar = 20;
	int delta_min_num = 0;
	double norm2_max = 0;

	// init
	memset(buf, 0, n0*sizeof(double));
	n = 0;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		if (s->weight < 0) { theMsg->printf("Error: subproblem weight cannot be negative!\n"); return 0; }
		if (s->weight == 0) continue;
		for (k=0; k<s->var_num; k++) buf[s->array[k]] += s->weight*s->weight;
		n += s->var_num;
	}
	for (k=0; k<n0; k++)
	{
		if (buf[k] <= 0) { theMsg->printf("Incorrect decomposition: not all elements are covered!\n"); return 0; }
	}

	double* vars = new double[2*n];
	double* g = vars + n;

	ReadBest(vars);

	z = theta = ProjectAndComputeSubgradient(vars, g, F_dummy);
	WriteBest(vars);

	gamma = 0;
	for (k=0; k<iter_max; k++)
	{
		double upper_bound = F_upper(user_arg);
		if (k==0 || upper_bound_best>upper_bound) upper_bound_best = upper_bound;
		if (k==0) delta = (upper_bound_best - z)*options.C;
		if (z>=upper_bound_best-gap_threshold) break;

	    if ( theWorkArea->wasInterrupted() ) break;
		theWorkArea->setProgressValue(((float)k)/iter_max);
		
		double norm2 = GetNorm2(g, n);
		if (norm2 == 0) break;
		if (norm2_max < norm2) norm2_max = norm2;
		double upper_bound_estimate = z+delta;
		//if (upper_bound_estimate > upper_bound_best) upper_bound_estimate = upper_bound_best;
		double lambda = options.alpha*(upper_bound_estimate-theta)/norm2;
		if (options.verbose && k%1000==0) theMsg->printf("%d\t%f\t%f\tstep=%f", k, theta, upper_bound, lambda);
		Add(vars, g, lambda, n);

		theta = ProjectAndComputeSubgradient(vars, g, F_dummy);

		if (theta > z)
		{
			if (options.verbose) //printf(" *");
			delta *= options.A;
			z = theta;
			WriteBest(vars);
			gamma = 0;
		}
		else 
		{
			delta *= options.B;
			if (gamma ++ >= gamma_bar)
			{
				if (options.verbose) theMsg->printf(" restart");
				gamma_bar += 10; if (gamma_bar > 50) gamma_bar = 50;
				ReadBest(vars);
				gamma = 0;

				theta = ProjectAndComputeSubgradient(vars, g, F_dummy);
				if (delta < (upper_bound_best - z)*0.01)
				{
					if (options.verbose) theMsg->printf("\nconverged?\n");
					break;
				}
			}
		}
		if (delta < (upper_bound_best - z)*options.C)
		{
			if (options.verbose) theMsg->printf(" !");
			if (delta_min_num ++ >= 30)
			{
				if (options.verbose) theMsg->printf("\nconverged?\n");
				break;
			}
			delta = (upper_bound_best - z)*options.C;
		}
		// if (options.verbose) printf("\n");
	}
	
	if (options.verbose) theMsg->printf("lower_bound=%f\tupper_bound=%f\n", z, upper_bound_best);

	delete [] vars;

	return z;
}
