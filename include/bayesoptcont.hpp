/**  \file bayesoptcont.hpp \brief Continuous Bayesian optimization */
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

#ifndef  _BAYESOPTCONT_HPP_
#define  _BAYESOPTCONT_HPP_

#include "boundingbox.hpp"
#include "bayesoptbase.hpp"
#include "inneroptimization.hpp"

namespace bayesopt  {

  /** \addtogroup BayesOpt */
  /**@{*/

  /**
   * \brief Bayesian optimization using different non-parametric
   * processes as distributions over surrogate functions. The
   * exploration spaces is assumed to be continous and box-bounded.
   */
  class BAYESOPT_API ContinuousModel: public BayesOptBase
  {
  public:
   
    /** Default constructor */
    ContinuousModel();

    /** 
     * Constructor
     * @param dim number of input dimensions
     * @param params set of parameters (see parameters.h)
     */
    ContinuousModel(size_t dim, bopt_params params);

    /**  Default destructor  */
    virtual ~ContinuousModel();
  
    /** Initialize the optimization process.  */
    void initializeOptimization();

    /** 
     * Once the optimization has been perfomed, return the optimal
     * point.
     */
    vectord getFinalResult();

    /** 
     * \brief Sets the bounding box. 
     *
     * @param lowerBound vector with the lower bounds of the hypercube
     * @param upperBound vector with the upper bounds of the hypercube
     * 
     * @return 0 if terminate successfully, nonzero otherwise
     */
    int setBoundingBox( const vectord &lowerBound,
			const vectord &upperBound);


  protected:

    /** 
     * \brief Print data for every step according to the verbose level
     * 
     * @param iteration iteration number 
     * @param xNext next point
     * @param yNext function value at next point
     */
    void plotStepData(size_t iteration, const vectord& xNext,
		      double yNext);

    /** \brief Sample a set of points to initialize the surrogate function.
     * It uses pure random sampling or uniform Latin Hypercube sampling.
     * @return error code
     */
    int sampleInitialPoints();


    /** 
     * \brief Returns the corresponding criteria of a series of queries
     * in the hypercube [0,1] in order to choose the best point to try
     * the next iteration.
     * 
     * @param query point in the hypercube [0,1] to evaluate the
     * Gaussian process
     * 
     * @return negative criteria (Expected Improvement, LCB,
     * I-optimality, etc.).
     */	
    //double evaluate( const vectord &query )
    //{  return evaluateCriteria(query); }; 

    
    /** 
     * \brief Wrapper for the target function normalize in the hypercube
     * [0,1]
     * @param query point to evaluate in [0,1] hypercube
     * @return actual return value of the target function
     */
    double evaluateSampleInternal( const vectord &query );
    
    /** 
     * \brief Wrapper of the innerOptimization class to find the optimal
     * point acording to the criteria.
     * 
     * @param xOpt optimal point
     * @return error code
     */
    int findOptimal(vectord &xOpt);

  private:
    utils::BoundingBox<vectord> *mBB;      ///< Bounding Box (input space limits)
    NLOPT_Optimization* cOptimizer;
  };
  
  /**@}*/


  inline double ContinuousModel::evaluateSampleInternal( const vectord &query )
  { 
    vectord unnormalizedQuery = mBB->unnormalizeVector(query);
    return evaluateSample(unnormalizedQuery);
  }; // evaluateSampleInternal

  inline int ContinuousModel::findOptimal(vectord &xOpt)
  { return cOptimizer->run(xOpt); };



}  //namespace bayesopt


#endif
