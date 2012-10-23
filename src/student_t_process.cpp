/*
-----------------------------------------------------------------------------
   Copyright (C) 2011 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/
#include <boost/math/distributions/students_t.hpp> // for student t distribution

#include "student_t_process.hpp"
#include "cholesky.hpp"
#include "trace_ublas.hpp"

using boost::numeric::ublas::inplace_solve;
using boost::numeric::ublas::lower_tag;
using boost::numeric::ublas::lower;

  
StudentTProcess::StudentTProcess(double noise):
  NonParametricProcess(noise)
{}  // Constructor



StudentTProcess::~StudentTProcess()
{} // Default destructor




double StudentTProcess::negativeLogLikelihood(size_t index)
{
  matrixd K = computeCorrMatrix();
  size_t n = K.size1();
  
  matrixd L(n,n);
  cholesky_decompose(K,L);

  vectord colU = (*mMean)(mGPXX);;

  //TODO: Replace by transform
  //  for (size_t ii=0; ii< n; ii++) 
  //  colU(ii) = 

  vectord alphU(colU);
  inplace_solve(L,alphU,lower_tag());
  double eta = inner_prod(colU,alphU);
  
  vectord alphY(mGPY);
  inplace_solve(L,alphY,lower_tag());
  double mu     = inner_prod(colU,alphY) / eta;
  double YInvRY = inner_prod(mGPY,alphY);
    
  double sigma = (YInvRY - mu*mu*eta) / (n-1);

  double negloglik = 0.5*( (n-1)*log(sigma) + trace(L) + log(eta) );

  return negloglik;
}


int StudentTProcess::prediction( const vectord &query,
				 double& yPred, double& sPred)
{
  size_t n = mGPXX.size();
  double uInvRr, rInvRr, rInvRy;
  double meanf = mMean->getMean(query);
  
  vectord colR = computeCrossCorrelation(query);
  double kn = (*mKernel)(query, query);
  
#if USE_CHOL
  vectord invRr(colR);
  inplace_solve(mL,invRr,lower_tag());
  rInvRr = inner_prod(invRr,invRr);
  uInvRr = inner_prod(mUInvR, invRr);  
  rInvRy = inner_prod(invRr, mInvRy );
#else
  vectord rInvR(n);
  noalias(rInvR) = prod(colR,mInvR);	
  rInvRr = inner_prod(rInvR,colR);
  uInvRr = inner_prod(mUInvR,colR);
  
  vectord yumu = mGPY - mMeanV*mMu;
  rInvRy = inner_prod( rInvR, yumu );
#endif

  yPred = meanf*mMu + rInvRy;
  sPred = sqrt( mSig * (kn - rInvRr + (meanf - uInvRr) * (meanf - uInvRr) 
			/ mUInvRUDelta ) );

  return n-1;
}
	

int StudentTProcess::precomputePrediction()
{
  size_t nSamples = mGPXX.size();
  double YInvRY;

  mInvRy.resize(nSamples,false);

#if USE_CHOL
  mUInvR = mMeanV;
  inplace_solve(mL,mUInvR,lower_tag());
  mUInvRUDelta = inner_prod(mUInvR,mUInvR);

  mInvRy = mGPY;
  inplace_solve(mL,mInvRy,lower_tag());
  mMu =  inner_prod(mUInvR,mInvRy) / mUInvRUDelta;
  YInvRY = inner_prod(mInvRy,mInvRy);
#else
  mUInvR = prod(mMeanV,mInvR);
  mUInvRUDelta = inner_prod(mUInvR,mMeanV);
  mMu =  inner_prod(mUInvR,mGPY) / mUInvRUDelta;
  noalias(mInvRy) = prod(mInvR,mGPY); 
  YInvRY = inner_prod(mGPY,mInvRy);
#endif
  
  mSig = (YInvRY - mMu*mMu*mUInvRUDelta) / (nSamples-1);

  return 1;
}


	
double StudentTProcess::negativeExpectedImprovement(const vectord& query,
						    size_t g)
{
  size_t dof = mGPXX.size() - 1;
  boost::math::students_t d(dof);

  double yPred,sPred;
  double yMin = getValueAtMinimum();
  prediction(query,yPred,sPred);

  double yDiff = yMin - yPred; 
  double yNorm = yDiff / sPred;
  
  assert((g != 1) && "Students t EI with exponent not yet supported.");
  return -( yDiff * cdf(d,yNorm) + 
	    (dof*sPred+yNorm*yDiff)/(dof-1) * pdf(d,yNorm) );

  
}  // negativeExpectedImprovement

double StudentTProcess::lowerConfidenceBound(const vectord& query,
					     double beta)
{    
  size_t n = mGPXX.size();
  double yPred,sPred;
  prediction(query,yPred,sPred);
  return yPred - beta*sPred/sqrt(n);
}  // lowerConfidenceBound

double StudentTProcess::negativeProbabilityOfImprovement(const vectord& query,
							 double epsilon)
{  
  size_t dof = mGPXX.size() - 1;
  boost::math::students_t d(dof);
  double yPred,sPred;
  double yMin = getValueAtMinimum();
  prediction(query,yPred,sPred);

  return -cdf(d,(yMin - yPred + epsilon)/sPred);
}  // negativeProbabilityOfImprovement
