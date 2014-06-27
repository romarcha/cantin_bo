/*
-------------------------------------------------------------------------
   This file is part of BayesOpt, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011-2014 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
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

#include "bayesopt.hpp"

#include <limits>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include "lhs.hpp"
#include "randgen.hpp"
#include "log.hpp"
#include "boundingbox.hpp"
#include "inneroptimization.hpp"



namespace bayesopt  {

  class CritCallback: public RBOptimizable
  {
  public:
    explicit CritCallback(ContinuousModel* model):mBO(model){};
    double evaluate(const vectord &query) 
    {
      return mBO->evaluateCriteria(query);
    }
  private:
    ContinuousModel* mBO;
  };
  
  ContinuousModel::ContinuousModel(size_t dim, bopt_params parameters):
    BayesOptBase(dim,parameters)
  { 
    mCallback.reset(new CritCallback(this));
    cOptimizer.reset(new NLOPT_Optimization(mCallback.get(),dim));
    cOptimizer->setAlgorithm(COMBINED);
    cOptimizer->setMaxEvals(parameters.n_inner_iterations);

    vectord lowerBound = zvectord(mDims);
    vectord upperBound = svectord(mDims,1.0);
    mBB.reset(new utils::BoundingBox<vectord>(lowerBound,upperBound));
  } // Constructor

  ContinuousModel::~ContinuousModel()
  {
    //    delete cOptimizer;
  } // Default destructor

  vectord ContinuousModel::getFinalResult()
  {
    return mBB->unnormalizeVector(getPointAtMinimum());
  }

  //////////////////////////////////////////////////////////////////////

  double ContinuousModel::evaluateSampleInternal( const vectord &query )
  { 
    const double yNext = evaluateSample(mBB->unnormalizeVector(query)); 
    if (yNext == HUGE_VAL)
      {
	throw std::runtime_error("Function evaluation out of range");
      }
    return yNext;
  }; 


  void ContinuousModel::findOptimal(vectord &xOpt)
  { 
    double minf = cOptimizer->run(xOpt);

    //Let's try some local exploration like spearmint
    randNFloat drawSample(mEngine,normalDist(0,0.001));
    for(size_t ii = 0;ii<5; ++ii)
      {
	vectord pert = getPointAtMinimum();
	for(size_t j=0; j<xOpt.size(); ++j)
	  {
	    pert(j) += drawSample();
	  }
	try
	  {
	    double minf2 = cOptimizer->localTrialAround(pert);	    
	    if (minf2<minf) 
	      {
		minf = minf2;
		FILE_LOG(logDEBUG) << "Local beats Global";
		xOpt = pert;
	      }
	  }
	catch(std::invalid_argument& e)
	  {
	    //We ignore this one
	  }
      }
  };

  vectord ContinuousModel::samplePoint()
  {	    
    randFloat drawSample(mEngine,realUniformDist(0,1));
    vectord Xnext(mDims);    
    for(vectord::iterator x = Xnext.begin(); x != Xnext.end(); ++x)
      {	
	*x = drawSample(); 
      }
    return Xnext;
  };


  void ContinuousModel::setBoundingBox(const vectord &lowerBound,
				       const vectord &upperBound)
  {
    // We don't change the bounds of the inner optimization because,
    // thanks to this bounding box model, everything is mapped to the
    // unit hypercube, thus the default inner optimization are just
    // right.
    mBB.reset(new utils::BoundingBox<vectord>(lowerBound,upperBound));
    
    FILE_LOG(logINFO) << "Bounds: ";
    FILE_LOG(logINFO) << lowerBound;
    FILE_LOG(logINFO) << upperBound;
  } //setBoundingBox



  void ContinuousModel::plotStepData(size_t iteration, const vectord& xNext,
			    double yNext)
  {
    if(mParameters.verbose_level >0)
      { 
	FILE_LOG(logINFO) << "Iteration: " << iteration+1 << " of " 
			  << mParameters.n_iterations << " | Total samples: " 
			  << iteration+1+mParameters.n_init_samples ;
	FILE_LOG(logINFO) << "Query: " << mBB->unnormalizeVector(xNext); ;
	FILE_LOG(logINFO) << "Query outcome: " << yNext ;
	FILE_LOG(logINFO) << "Best query: " 
			  << mBB->unnormalizeVector(getPointAtMinimum()); 
	FILE_LOG(logINFO) << "Best outcome: " <<  getValueAtMinimum();
      }
  } //plotStepData

  void ContinuousModel::sampleInitialPoints(matrixd& xPoints, vectord& yPoints)
  {   
    utils::samplePoints(xPoints,mParameters.init_method,mEngine);

    for(size_t i = 0; i < yPoints.size(); i++)
      {
	const vectord sample = row(xPoints,i);
	yPoints(i) = evaluateSampleInternal(sample);
      }
  }
}  //namespace bayesopt
