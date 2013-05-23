/**  \file bayesoptextras.h 
              \brief Helper functions to Matlab/Octave wrappers. */
/*
-----------------------------------------------------------------------------
   This file is part of BayesOptimization, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
   BayesOptimization is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BayesOptimization is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BayesOptimization.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#ifndef __BAYESOPTEXTRAS_H__
#define __BAYESOPTEXTRAS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mex.h>

#include "parameters.h"
#include "bayesoptwpr.h"

#define CHECK0(cond, msg) if (!(cond)) mexErrMsgTxt(msg);

static void struct_value(const mxArray *s, const char *name, double *result)
{
  mxArray *val = mxGetField(s, 0, name);
  if (val) 
    {
      if(!(mxIsNumeric(val) && !mxIsComplex(val) 
	   && mxGetM(val) * mxGetN(val) == 1))
	{
	  mexErrMsgTxt("param fields must be real scalars");
	}
      else
	{
	  *result = mxGetScalar(val);
	}
    }
  else
    {
      mexPrintf("Field %s not found. Default not modified.\n", name);
    }
  return;
}

static void struct_array(const mxArray *s, const char *name, size_t *n, double *result)
{
  mxArray *val = mxGetField(s, 0, name);
  if (val) 
    {
      if(!(mxIsNumeric(val) && !mxIsComplex(val)))
	{
	  mexErrMsgTxt("Param fields must be vector");
	}
      else
	{	   
	  *n = mxGetM(val) * mxGetN(val);
	  memcpy(result, mxGetPr(val), *n * sizeof(double));
	}
    }
  else
    {
      mexPrintf("Field %s not found. Default not modified.\n", name);
    }
  return;
}


static void struct_size(const mxArray *s, const char *name, size_t *result)
{
  mxArray *val = mxGetField(s, 0, name);
  if (val) 
    {
      if(!(mxIsNumeric(val) && !mxIsComplex(val) 
	   && mxGetM(val) * mxGetN(val) == 1))
	{
	  mexErrMsgTxt("param fields must be real scalars");
	}
      else
	{
	  *result = (size_t) mxGetScalar(val);
	}
    }
  else
    {
      mexPrintf("Field %s not found. Default not modified.\n", name);
    }
  return;
}


static void struct_string(const mxArray *s, const char *name, char* result)
{
  mxArray *val = mxGetField(s, 0, name);
  char *valstr;

  if (val) {
    if( mxIsChar(val) ) {
      if ( mxGetString(val, result, 1+(mxGetM(val) * mxGetN(val)))) {
	  mexErrMsgTxt("Error loading string.");
	}
    } else {
      mexErrMsgTxt("Method name must be a string");
    }
  } else {
    mexPrintf("Field %s not found. Default not modified.\n", name);
  }
  return;
}


#define FLEN 128 /* max length of user function name */
#define MAXRHS 2 /* max nrhs for user function */
typedef struct {
     char f[FLEN];
     mxArray *plhs[2];
     mxArray *prhs[MAXRHS];
     int xrhs, nrhs;
     int verbose, neval;
} user_function_data;



static double user_function(unsigned n, const double *x,
			    double *gradient, /* NULL if not needed */
			    void *d_)
{
  user_function_data *d = (user_function_data *) d_;
  double f;

  
  d->plhs[0] = d->plhs[1] = NULL;

  memcpy(mxGetPr(d->prhs[d->xrhs]), x, n * sizeof(double));

  CHECK0(0 == mexCallMATLAB(gradient ? 2 : 1, d->plhs, 
			   d->nrhs, d->prhs, d->f),
	"error calling user function");

  CHECK0(mxIsNumeric(d->plhs[0]) && !mxIsComplex(d->plhs[0]) 
	 && (mxGetM(d->plhs[0]) * mxGetN(d->plhs[0]) == 1),
	"user function must return real scalar");
  f = mxGetScalar(d->plhs[0]);
  mxDestroyArray(d->plhs[0]);

  if (gradient) {
     CHECK0(mxIsDouble(d->plhs[1]) && !mxIsComplex(d->plhs[1])
	   && (mxGetM(d->plhs[1]) == 1 || mxGetN(d->plhs[1]) == 1)
	   && mxGetM(d->plhs[1]) * mxGetN(d->plhs[1]) == n,
	   "gradient vector from user function is the wrong size");
     memcpy(gradient, mxGetPr(d->plhs[1]), n * sizeof(double));
     mxDestroyArray(d->plhs[1]);
  }
  d->neval++;

  if (d->verbose) mexPrintf("Optimize eval #%d: %g\n", d->neval, f);
  return f;
}

static bopt_params load_parameters(const mxArray* params)
{  
  
  /* See parameters.h for the available options */
  
  char log_str[100], k_s_str[100];
  char l_str[100];
  size_t n_hp_test, n_coef_test;

  bopt_params parameters = initialize_parameters_to_default();

  n_hp_test = parameters.kernel.n_hp;
  n_coef_test = parameters.mean.n_coef;

  struct_size(params,"n_iterations", &parameters.n_iterations);
  struct_size(params,"n_inner_iterations", &parameters.n_inner_iterations);
  struct_size(params, "n_init_samples", &parameters.n_init_samples);
  struct_size(params, "n_iter_relearn", &parameters.n_iter_relearn);
  
  struct_size(params, "verbose_level", &parameters.verbose_level);
  struct_string(params, "log_filename", parameters.log_filename);

  struct_string(params, "surr_name", parameters.surr_name);

  struct_value(params, "sigma_s", &parameters.sigma_s);
  struct_value(params, "noise", &parameters.noise);
  struct_value(params, "alpha", &parameters.alpha);
  struct_value(params, "beta",  &parameters.beta);
  

  strcpy( l_str, learn2str(parameters.l_type));
  struct_string(params, "l_type", l_str);
  parameters.l_type = str2learn(l_str);

  struct_value(params, "epsilon",  &parameters.epsilon);

  struct_string(params, "crit_name", parameters.crit_name);
  struct_array(params, "crit_params", &parameters.n_crit_params, 
	       &parameters.crit_params[0]);

  /* Kernel parameters */
  struct_string(params, "kernel_name", parameters.kernel.name);
  struct_array(params, "kernel_hp_mean", &parameters.kernel.n_hp, 
	       &parameters.kernel.hp_mean[0]);
  struct_array(params, "kernel_hp_std", &n_hp_test, 
	       &parameters.kernel.hp_std[0]);

  CHECK0(parameters.kernel.n_hp == n_hp_test, 
	 "Error processing kernel parameters");

  /* Mean function parameters */
  struct_string(params, "mean_name", parameters.mean.name);
  struct_array(params, "mean_coef_mean", &parameters.mean.n_coef, 
	       &parameters.mean.coef_mean[0]);

  struct_array(params, "mean_coef_std", &n_coef_test, 
	       &parameters.mean.coef_std[0]);

  CHECK0(parameters.mean.n_coef == n_coef_test, 
	 "Error processing mean parameters");


  return parameters;
}

#endif
