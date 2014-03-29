/** \file kernelregressor.hpp 
    \brief Nonparametric process abstract module */
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


#ifndef __NONPARAMETRICPROCESS_HPP__
#define __NONPARAMETRICPROCESS_HPP__

#include "cholesky.hpp"
#include "nonparametricprocess.hpp"
#include "kernel_functors.hpp"

namespace bayesopt
{

  /** \addtogroup NonParametricProcesses 
   *  \brief Set of nonparametric processes (Gaussian process, Student's
   *  t process, etc.) for surrogate modelling
   */
  /**@{*/


  /**
   * \brief Abstract class to implement non-parametric processes
   */
  class KernelRegressor: public NonParametricProcess
  {
  public:
    KernelRegressor(size_t dim, bopt_params parameters, const Dataset& data, randEngine& eng);
    virtual ~KernelRegressor();

    /** 
     * \brief Updates the kernel parameters with a point estimate
     * (empirical Bayes) or a full Bayesian distribution
     */
    virtual void updateKernelParameters() = 0;

    /** 
     * \brief Computes the initial surrogate model and updates the
     * kernel parameters estimation. 
     *
     * This function requires to recompute the full Kernel matrix (and
     * its decomposition).  Use it with precaution.
     */
    void fitSurrogateModel();

    /** 
     * \brief Sequential update of the surrogate model by adding a new
     * row to the Kernel matrix, more precisely, to its Cholesky
     * decomposition. 
     * 
     * It assumes that the kernel hyperparemeters do not change.
     */   
    void updateSurrogateModel(const vectord &Xnew);


    // Getters and setters
    double getSignalVariance();
    size_t nHyperParameters();
    vectord getHyperParameters();
    void setHyperParameters(const vectord& theta);

    /** Sets the kind of learning methodology for kernel hyperparameters */
    //void setLearnType(learning_type l_type);

  protected:

    /** Precompute some values of the prediction that do not depends
     * on the query. */
    virtual void precomputePrediction() = 0;

    /** Computes the Correlation (Kernel or Gram) matrix */
    void computeCorrMatrix(matrixd& corrMatrix);

    /** Computes the Correlation (Kernel or Gram) matrix */
    matrixd computeCorrMatrix();

    /** Computes the derivative of the correlation matrix with respect
     *	to the dth hyperparameter */
    matrixd computeDerivativeCorrMatrix(int dth_index);
    vectord computeCrossCorrelation(const vectord &query);
    double computeSelfCorrelation(const vectord& query);

    /** Computes the Cholesky decomposition of the Correlation matrix */
    void computeCholeskyCorrelation();


  protected:
    matrixd mL;             ///< Cholesky decomposition of the Correlation matrix
    score_type mScoreType;
    //learning_type mLearnType;
    KernelModel mKernel;

  private:
    /** Adds a new point to the Cholesky decomposition of the
     * Correlation matrix. */
    void addNewPointToCholesky(const vectord& correlation,
			      double selfcorrelation);


    const double mRegularizer;   ///< Std of the obs. model (also used as nugget)
  };

  //// Inline methods
  inline void KernelRegressor::fitSurrogateModel()
  {
    //updateKernelParameters();
    computeCholeskyCorrelation();
    precomputePrediction(); 
  };

  inline size_t KernelRegressor::nHyperParameters()
  { return mKernel.nHyperParameters(); }

  inline vectord KernelRegressor::getHyperParameters()
  { return mKernel.getHyperParameters(); }

  inline void KernelRegressor::setHyperParameters(const vectord &theta)
  { mKernel.setHyperParameters(theta); };

  // inline void KernelRegressor::setLearnType(learning_type l_type) 
  // { mLearnType = l_type; };

  inline void KernelRegressor::computeCorrMatrix(matrixd& corrMatrix)
  { mKernel.computeCorrMatrix(mData.mX,corrMatrix,mRegularizer); }

  inline  matrixd KernelRegressor::computeCorrMatrix()
  {
    const size_t nSamples = mData.getNSamples();
    matrixd corrMatrix(nSamples,nSamples);
    mKernel.computeCorrMatrix(mData.mX,corrMatrix,mRegularizer);
    return corrMatrix;
  }

  inline vectord KernelRegressor::computeCrossCorrelation(const vectord &query)
  { return mKernel.computeCrossCorrelation(mData.mX,query); }

  inline double KernelRegressor::computeSelfCorrelation(const vectord& query)
  { return mKernel.computeSelfCorrelation(query); }

  inline void KernelRegressor::addNewPointToCholesky(const vectord& correlation,
							  double selfcorrelation)
  {
    vectord newK(correlation);
    utils::append(newK, selfcorrelation);
    utils::cholesky_add_row(mL,newK);
  }

  /**@}*/
  
} //namespace bayesopt

#endif
