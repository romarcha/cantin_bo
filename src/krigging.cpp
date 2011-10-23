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

#include "krigging.hpp"
#include "lhs.hpp"
#include "randgen.hpp"
#include "basicgaussprocess.hpp"


SKO::SKO( double theta, double noise,
	  size_t nIter, double alpha, 
	  double beta,  double delta,
	  NonParametricProcess* gp):
  mMaxIterations(nIter), mMaxDim(MAX_DIM),
  mVerbose(0)
{ 
  crit_name = c_gp_hedge;
  if (gp == NULL)
    mGP = new BasicGaussianProcess(theta,noise);
  else
    mGP = gp;

} // Constructor

SKO::SKO( gp_params params,
	  size_t nIter,
	  NonParametricProcess* gp):
  mMaxIterations(nIter), mMaxDim(MAX_DIM),
  mVerbose(0)
{
  crit_name = c_gp_hedge;
  if (gp == NULL)
    mGP = new BasicGaussianProcess(params.theta,params.noise);
  else
    mGP = gp;
} // Constructor


SKO::~SKO()
{} // Default destructor


int SKO::optimize( vectord &bestPoint)
{
  size_t dim = bestPoint.size();
  vectord lowerBound = zvectord(dim);
  vectord upperBound = svectord(dim,1.0);
  
  return optimize(bestPoint,lowerBound,upperBound);
}

int SKO::optimize( vectord &bestPoint,
		   vectord &lowerBound,
		   vectord &upperBound)
{
  mVerbose = 1;

  crit.resetHedgeValues();
 
  mLowerBound = lowerBound;
  mRangeBound = upperBound - lowerBound;

  size_t nDims = bestPoint.size();
  
  if ( nDims > mMaxDim )
    { 
      std::cout << "Warning!! Too many dimensions!! " << std::endl;
      std::cout << "This algorithm is efficient up to " << mMaxDim 
		<< " dimensions." << std::endl;
    }

  vectord xNext(nDims);
  double yNext;
  size_t nLHSSamples = N_LHS_EVALS_PER_DIM * nDims;

  if (nLHSSamples > MAX_LHS_EVALUATIONS)
    nLHSSamples = MAX_LHS_EVALUATIONS;
  
  if (  ( mMaxIterations > (MAX_ITERATIONS - nLHSSamples) )  
	|| ( mMaxIterations <= 0) )
    mMaxIterations = MAX_ITERATIONS - nLHSSamples;

  std::cout << "Sampling initial points..." << std::endl;
  sampleInitialPoints(nLHSSamples, nDims, true);
  std::cout << "DONE" << std::endl;

  for (size_t ii = 0; ii < mMaxIterations; ii++)
    {      
      // Find what is the next point.
      innerOptimize(xNext);
    
      if(mVerbose >0)
	{ 
	  std::cout << "Iteration " << ii+1 << std::endl;
	  std::cout << "Trying: " << xNext << std::endl;
	  std::cout << "Best: " << mGP->getPointAtMinimum() << std::endl; 
	  std::cout << "Best outcome: " <<  mGP->getValueAtMinimum() <<  std::endl; 
	}
     
      yNext = evaluateNormalizedSample(xNext);
      mGP->addNewPointToGP(xNext,yNext);     
    }

  bestPoint = mGP->getPointAtMinimum();

  return 1;
} // optimize

int SKO::sampleInitialPoints( size_t nSamples, size_t nDims,
			      bool useLatinBox)
{
  /** \brief Sample a set of points to initialize GP fit
   * Use pure random sampling or uniform Latin Hypercube sampling
   * as appeared in Jones EGO
   */
   
  matrixd xPoints(nSamples,nDims);
  vectord yPoints(nSamples);
  vectord sample(nDims);
  randEngine mtRandom(100u);
 
  if (useLatinBox)
      lhs(xPoints, mtRandom);
  else
      uniformSampling(xPoints, mtRandom);

  for(size_t i = 0; i < nSamples; i++)
    {
      sample = row(xPoints,i);
      if(mVerbose >0)
	std::cout << sample << std::endl;
      yPoints(i) = evaluateNormalizedSample(sample);
    }

  mGP->setSamples(xPoints,yPoints);
  mGP->fitGP();

  return 1;
} // sampleInitialPoints


int SKO::nextPoint(vectord &Xnext)
{
  crit.resetAnnealValues();
  if (crit_name == c_gp_hedge)
    {
      vectord best_ei(Xnext);
      vectord best_lcb(Xnext);
      vectord best_poi(Xnext);
      double r_ei,r_lcb,r_poi,foo;

      crit.setCriterium(c_ei);
      innerOptimize(best_ei);
      mGP->prediction(best_ei,r_ei,foo);
      
      crit.setCriterium(c_lcb);
      innerOptimize(best_lcb);
      mGP->prediction(best_lcb,r_lcb,foo);

      crit.setCriterium(c_poi);
      innerOptimize(best_poi);
      mGP->prediction(best_poi,r_poi,foo);

      criterium_name better = crit.update_hedge(r_ei,r_lcb,r_poi);
      switch(better)
	{
	case c_ei: Xnext = best_ei; break;
	case c_lcb: Xnext = best_lcb; break;
	case c_poi: Xnext = best_poi; break;
	default: return -1;
	}
      return 1;
    }
  else
    {
      crit.setCriterium(crit_name);
      return innerOptimize(Xnext);
    }
}


double SKO::evaluateCriteria(const vectord &query)
{
  bool reachable = checkReachability(query);
  if (!reachable)
    return 0.0;

  return crit.evaluate(*mGP,query);
       
}  // evaluateCriteria


