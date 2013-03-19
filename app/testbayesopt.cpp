#include <ctime>
#include "bayesoptwpr.h"             // For the C-AP
#include "bayesoptcont.hpp"              // For the C++-API


/* Function to be used for C-API testing */
double testFunction(unsigned int n, const double *x,
		    double *gradient, /* NULL if not needed */
		    void *func_data)
{
  double f = 10.;
  for (unsigned int i = 0; i < n; ++i)
    {
      f += (x[i] - .53) * (x[i] - .53);
    }
  return f;
}

/* Class to be used for C++-API testing */
class TestEGO: public bayesopt::BayesOptContinuous
{
 public:

  TestEGO(size_t dim,bopt_params param):
    bayesopt::BayesOptContinuous(dim,param) {}

  double evaluateSample( const vectord &Xi ) 
  {
    double x[100];
    for (size_t i = 0; i < Xi.size(); ++i)
	x[i] = Xi(i);
    return testFunction(Xi.size(),x,NULL,NULL);
  };


  bool checkReachability( const vectord &query )
  { return true; };
 
};


int main(int nargs, char *args[])
{    
  int n = 10;                   // Number of dimensions

  // Common configuration
  // See parameters.h for the available options
  // If we initialize the struct with the DEFAUL_PARAMS,
  // the we can optionally change only few of them 
  bopt_params par = initialize_parameters_to_default();

  par.theta[0] = KERNEL_THETA;
  par.n_theta = 1;
  par.alpha = PRIOR_ALPHA;
  par.beta = PRIOR_BETA;
  par.delta = PRIOR_DELTA_SQ;
  par.noise = DEFAULT_NOISE;
  par.c_name = C_EI;
  par.s_name = S_STUDENT_T_PROCESS_JEFFREYS;
  par.k_name = K_MATERN_ISO3;
  par.m_name = M_ZERO;
  par.n_iterations = 200;       // Number of iterations
  par.n_init_samples = 50;
  par.verbose_level = 2;
  /*******************************************/

  clock_t start, end;
  double diff,diff2;

  std::cout << "Running C++ interface" << std::endl;
  // Configure C++ interface

  TestEGO gp_opt(n,par);
  vectord result(n);

  // Run C++ interface
  start = clock();
  gp_opt.optimize(result);
  end = clock();
  diff = (double)(end-start) / (double)CLOCKS_PER_SEC;
  /*******************************************/


  std::cout << "Running C inferface" << std::endl;
  
  // Configure C interface
  double u[128], x[128], l[128], fmin[128];

  for (int i = 0; i < n; ++i) {
    l[i] = 0.;    u[i] = 1.;
  }

  // Run C interface
  start = clock();
  bayes_optimization(n,&testFunction,NULL,l,u,x,fmin,par);
  end = clock();
  diff2 = (double)(end-start) / (double)CLOCKS_PER_SEC;
  /*******************************************/


  // Results
  std::cout << "Final result C++: " << result << std::endl;
  std::cout << "Elapsed time in C++: " << diff << " seconds" << std::endl;

  std::cout << "Final result C: [" << n <<"](" << x[0];
  for (int i = 1; i < n; ++i )
    std::cout << "," << x[i];
  
  std::cout << ")" << std::endl;
  std::cout << "Elapsed time in C: " << diff2 << " seconds" << std::endl;

  std::cout << kernel2str(par.k_name)<< std::endl;
}

