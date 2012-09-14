#include <ctime>
#include "bayesoptwpr.h"             // For the C-AP
#include "bayesoptcont.hpp"              // For the C++-API


/* Function to be used for C-API testing */
double testFunction(unsigned int n, double *x,
		    double *gradient, /* NULL if not needed */
		    void *func_data)
{
  double f = 10.;
  for (unsigned int i = 0; i < n; ++i)
    {
      f += (x[i] - .53f) * (x[i] - .53f);
    }
  return f;
}

/* Class to be used for C++-API testing */
class TestEGO: public SKO_CONT
{
 public:

  TestEGO(sko_params param):
    SKO_CONT(param) {}

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
  int n = 1;                   // Number of dimensions

  // Common configuration
  // See ctypes.h for the available options
  // If we initialize the struct with the DEFAUL_PARAMS,
  // the we can optionally change only few of them 
  sko_params par = DEFAULT_PARAMS;

  par.theta = KERNEL_THETA;
  par.alpha = PRIOR_ALPHA;
  par.beta = PRIOR_BETA;
  par.delta = PRIOR_DELTA_SQ;
  par.noise = DEFAULT_NOISE;
  par.c_name = C_EI;
  par.s_name = S_GAUSSIAN_PROCESS;
  par.k_name = K_SE_ISO;
  par.n_iterations = 10;       // Number of iterations
  par.n_init_samples = 30;
  /*******************************************/

  clock_t start, end;
  double diff,diff2;

  std::cout << "Running C++ interface" << std::endl;
  // Configure C++ interface

  TestEGO gp_opt(par);
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
  std::cout << "Final result C: (";
  for (int i = 0; i < n; i++ )
    std::cout << x[i] << ", ";
  
  std::cout << ")" << std::endl;

  std::cout << "Elapsed time in C++: " << diff << " seconds" << std::endl;
  std::cout << "Elapsed time in C: " << diff2 << " seconds" << std::endl;
}

