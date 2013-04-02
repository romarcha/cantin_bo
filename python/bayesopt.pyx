## \file bayesopt.pyx \brief Cython wrapper for the BayesOpt Python API
# -------------------------------------------------------------------------
#    This file is part of BayesOpt, an efficient C++ library for
#    Bayesian optimization.
#
#    Copyright (C) 2011-2013 Ruben Martinez-Cantin <rmcantin@unizar.es>
#
#    BayesOpt is free software: you can redistribute it and/or modify it
#    under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    BayesOpt is distributed in the hope that it will be useful, but
#    WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with BayesOpt.  If not, see <http://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------


import numpy as np
cimport numpy as np
#from python_ref cimport Py_INCREF, Py_DECREF
from cpython cimport Py_INCREF, Py_DECREF

cdef extern from *:
    ctypedef double* const_double_ptr "const double*"

###########################################################################
cdef extern from "parameters.h":

    ctypedef enum kernel_name:
        pass

    ctypedef enum mean_name:
        pass

    ctypedef enum criterium_name:
        pass

    ctypedef enum surrogate_name:
        pass

    ctypedef enum learning_type:
        pass

    ctypedef struct kernel_parameters:
        char*  name
        double* theta
        double* s_theta
        unsigned int n_theta

    ctypedef struct mean_parameters:
        char* name
        double* mu
        double* s_mu
        unsigned int n_mu

    ctypedef struct bopt_params:
        unsigned int n_iterations, n_init_samples, verbose_level
        char* log_filename
        surrogate_name surr_name
        double alpha, beta
        double noise
        learning_type l_type
        kernel_parameters kernel
        mean_parameters mean
        char* crit_name

    kernel_name str2kernel(char* name)
    criterium_name str2crit(char* name)
    surrogate_name str2surrogate(char* name)
    mean_name str2mean(char* name)
    learning_type str2learn(char* name)

    char* kernel2str(kernel_name name)
    char* crit2str(criterium_name name)
    char* surrogate2str(surrogate_name name)
    char* mean2str(mean_name name)
    char* learn2str(learning_type name)

    bopt_params initialize_parameters_to_default()

###########################################################################
cdef extern from "bayesoptwpr.h":
    ctypedef double (*eval_func)(unsigned int n, const_double_ptr x,
                                 double *gradient, void *func_data)

    int bayes_optimization(int nDim, eval_func f, void* f_data,
                           double *lb, double *ub, double *x,
                           double *minf,
                           bopt_params params)

    int bayes_optimization_disc(int nDim, eval_func f, void* f_data,
                                double *valid_x, size_t n_points,
                                double *x, double *minf,
                                bopt_params parameters)

###########################################################################
cdef bopt_params dict2structparams(dict dparams):

    params = initialize_parameters_to_default()

    params.n_iterations = dparams.get('n_iterations',params.n_iterations)
    params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)
    params.verbose_level = dparams.get('verbose_level',params.verbose_level)

    logname = dparams.get('log_filename',params.log_filename)
    params.log_filename = logname

    surrogate = dparams.get('surr_name', None)
    if surrogate is not None:
        params.surr_name = str2surrogate(surrogate)

    learning = dparams.get('learning_type', None)
    if learning is not None:
        params.l_type = str2learn(learning)


    params.alpha = dparams.get('alpha',params.alpha)
    params.beta = dparams.get('beta',params.beta)
    params.noise = dparams.get('noise',params.noise)

    theta = dparams.get('theta',None)
    stheta = dparams.get('s_theta',None)
    if theta is not None and stheta is not None:
        params.kernel.n_theta = len(theta)
        for i in range(0,params.kernel.n_theta):
            params.kernel.theta[i] = theta[i]
            params.kernel.s_theta[i] = stheta[i]


    mu = dparams.get('mu',None)
    smu = dparams.get('s_mu',None)
    if mu is not None and smu is not None:
        params.mean.n_mu = len(mu)
        for i in range(0,params.mean.n_mu):
            params.mean.mu[i] = mu[i]
            params.mean.s_mu[i] = smu[i]


    kname = dparams.get('kernel_name',params.kernel.name)
    params.kernel.name = kname;

    mname = dparams.get('mean_name',params.mean.name)
    params.mean.name = mname

    cname = dparams.get('crit_name',params.crit_name)
    params.crit_name = cname

    return params

cdef double callback(unsigned int n, const_double_ptr x,
                     double *gradient, void *func_data):
    x_np = np.zeros(n)
    for i in range(0,n):
        x_np[i] = <double>x[i]
        result = (<object>func_data)(x_np)
    return result


def initialize_params():
    params = {
        "theta"  : [1.0],
        "s_theta": [1.0],
        "n_theta": 1,
        "mu"     : [1.0],
        "s_mu"   : [1.0],
        "n_mu"   : 1,
        "alpha"  : 1.0,
        "beta"   : 1.0,
        "noise"  : 0.001,
        "crit_name" : "cEI",
        "surr_name" : "GAUSSIAN_PROCESS" ,
        "kernel_name" : "kMaternISO3",
        "mean_name" : "mZero",
        "learning_type" : "L_MAP",
        "n_iterations"   : 300,
        "n_init_samples" : 30,
        "verbose_level"  : 1,
        "log_filename"   : "bayesopt.log"
        }
    return params

def optimize(f, int nDim, np.ndarray[np.double_t] np_lb,
             np.ndarray[np.double_t] np_ub, dict dparams):

    cdef bopt_params params = dict2structparams(dparams)
    cdef double minf[1000]
    cdef np.ndarray np_x = np.zeros([nDim], dtype=np.double)

    cdef np.ndarray[np.double_t, ndim=1, mode="c"] lb
    cdef np.ndarray[np.double_t, ndim=1, mode="c"] ub
    cdef np.ndarray[np.double_t, ndim=1, mode="c"] x

    lb = np.ascontiguousarray(np_lb,dtype=np.double)
    ub = np.ascontiguousarray(np_ub,dtype=np.double)
    x  = np.ascontiguousarray(np_x,dtype=np.double)

    Py_INCREF(f)

    error_code = bayes_optimization(nDim, callback, <void *> f,
                                    &lb[0], &ub[0], &x[0], minf, params)

    Py_DECREF(f)
    min_value = minf[0]
    return min_value,np_x,error_code


def optimize_discrete(f, np.ndarray[np.double_t,ndim=2] np_valid_x,
                      dict dparams):

    nDim = np_valid_x.shape[1]

    cdef bopt_params params = dict2structparams(dparams)
    cdef double minf[1000]
    cdef np.ndarray np_x = np.zeros([nDim], dtype=np.double)

    cdef np.ndarray[np.double_t, ndim=1, mode="c"] x
    cdef np.ndarray[np.double_t, ndim=2, mode="c"] valid_x

    x  = np.ascontiguousarray(np_x,dtype=np.double)
    valid_x = np.ascontiguousarray(np_valid_x,dtype=np.double)

    Py_INCREF(f)

    error_code = bayes_optimization_disc(nDim, callback, <void *> f,
                                         &valid_x[0,0], np_valid_x.shape[0],
                                         &x[0], minf, params)

    Py_DECREF(f)
    min_value = minf[0]
    return min_value,np_x,error_code
