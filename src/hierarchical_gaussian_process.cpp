/*
-------------------------------------------------------------------------
   This file is part of BayesOpt, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011-2013 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
   BayesOpt is free software: you can redistribute it and/or modify it 
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BayesOpt is distributed in the hope that it will be useful, but 
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BayesOpt.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------
*/
#include "hierarchical_gaussian_process.hpp"
#include "cholesky.hpp"
#include "trace_ublas.hpp"

namespace bayesopt
{
  namespace ublas = boost::numeric::ublas;

  HierarchicalGaussianProcess::HierarchicalGaussianProcess(size_t dim, 
							   bopt_params params):
    EmpiricalBayesProcess(dim, params) {};

  double HierarchicalGaussianProcess::negativeTotalLogLikelihood()
  {
    /*This is the restricted version. For the unrestricted, make p=0
      and remove the last term of loglik*/

    const matrixd K = computeCorrMatrix();
    const size_t n = K.size1();
    const size_t p = mFeatM.size1(); 
    matrixd L(n,n);
    utils::cholesky_decompose(K,L);

    matrixd KF(ublas::trans(mFeatM));
    inplace_solve(L,KF,ublas::lower_tag());
    
    matrixd FKF = prod(ublas::trans(KF),KF);
    matrixd L2(p,p);
    utils::cholesky_decompose(FKF,L2);

    vectord Ky(mData.mY);
    inplace_solve(L,Ky,ublas::lower_tag());

    vectord wML = prod(Ky,KF);
    utils::cholesky_solve(L2,wML,ublas::lower());

    vectord alpha = mData.mY - prod(wML,mFeatM);
    inplace_solve(L,alpha,ublas::lower_tag());
    double sigma = ublas::inner_prod(alpha,alpha)/(n-p);

    double loglik = .5*(n-p)*log(ublas::inner_prod(alpha,alpha)) 
      + utils::log_trace(L) + utils::log_trace(L2);
    return loglik;
  }
}
  
