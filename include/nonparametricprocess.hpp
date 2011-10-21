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

#ifndef __NONPARAMETRICPROCESS_HPP__
#define __NONPARAMETRICPROCESS_HPP__

#include "specialtypes.hpp"
#include "cholesky.hpp"


class NonParametricProcess
{
public:
  NonParametricProcess()
  { mMinIndex = 0; mMaxIndex = 0; }
  
  virtual ~NonParametricProcess(){ }

  /** 
   * Function that returns the prediction of the GP for a query point
   * in the hypercube [0,1].
   * 
   * @param query point in the hypercube [0,1] to evaluate the Gaussian process
   * @param yPred mean of the predicted Gaussian distribution
   * @param sPred std of the predicted Gaussian distribution
   * 
   * @return error code.
   */	
  virtual int prediction(const vectord &query,
			 double& yPred, double& sPred)
  {return 1;}

  /** 
   *  Computes the GP based on mGPXX
   *  This function is hightly inefficient O(N^3). Use it only at 
   *  the begining
   * 
   * @return error code
   */
  virtual int fitGP()
  { return 1;}

  /** 
   *  Add new point efficiently using Matrix Decomposition Lemma
   *  for the inversion of the correlation matrix. Maybe it is faster
   *  to just construct and invert a new matrix each time.
   * 
   * @return error code
   */   
  virtual int addNewPointToGP( const vectord &Xnew,
			       double Ynew)
  {return 1;}


  // Getters and setters
  inline void setSamples(matrixd x, vectord y)
  {
    size_t nPoints = x.size1();
    
    for (size_t i=0; i<nPoints; ++i)
      mGPXX.push_back(row(x,i));

    mGPY = y;
  }

  inline void addSample(vectord x, double y)
  {
    mGPXX.push_back(x);
    mGPY.resize(mGPY.size()+1);  mGPY(mGPY.size()-1) = y;
  }

  inline vectord getPointAtMinimum()
  { return mGPXX[mMinIndex]; }

  inline double getValueAtMinimum()
  { return mGPY(mMinIndex); }


protected:
  virtual double correlationFunction( const vectord &x1,const vectord &x2, 
				      size_t param_index = 0)
  {return 0.0;}

  virtual double meanFunction( const vectord &x, size_t param_index = 0 )  
  {return 0.0;}

  /** 
   * Precompute some values of the prediction that do not depends on the query
   * 
   * @return error code
   */
  virtual int precomputeGPParams()
  {return 1;}


  /** 
   * Computes the inverse of the Correlation (Kernel or Gram) matrix  
   * 
   * @return error code
   */
  int computeInverseCorrMatrix( double noise )
  {
    size_t nSamples = mGPXX.size();
    if ( (nSamples != mInvR.size1()) || (nSamples != mInvR.size2()) )
      mInvR.resize(nSamples,nSamples);
    
    matrixd corrMatrix = computeCorrMatrix(noise);

    //return InvertMatrix(corrMatrix,mInvR);
    return inverse_cholesky(corrMatrix,mInvR);
  }

  matrixd computeCorrMatrix( double noise , size_t dth_index = 0)
  {
    size_t nSamples = mGPXX.size();
    matrixd corrMatrix(nSamples,nSamples);
  
    for (size_t ii=0; ii< nSamples; ii++)
      {
	for (size_t jj=0; jj < ii; jj++)
	  {
	    corrMatrix(ii,jj) = correlationFunction(mGPXX[ii], mGPXX[jj], dth_index);
	    corrMatrix(jj,ii) = corrMatrix(ii,jj);
	  }
	corrMatrix(ii,ii) = correlationFunction(mGPXX[ii],mGPXX[ii], dth_index);
	if (dth_index == 0) 
	  corrMatrix(ii,ii) += noise;
      }
    return corrMatrix;
  }


  inline vectord computeCrossCorrelation(const vectord &query)
  {
    vectord knx(mGPXX.size());

    //TODO: Replace by transform
    for (size_t ii=0; ii<mGPXX.size(); ++ii)
	knx(ii) = correlationFunction(mGPXX[ii], query);

    return knx;
  }


  inline void checkBoundsY( size_t i )
  {
    if ( mGPY(mMinIndex) > mGPY(i) )       mMinIndex = i;
    else if ( mGPY(mMaxIndex) < mGPY(i) )  mMaxIndex = i;
  };

protected:
  vecOfvec mGPXX;                     // TODO:Data inputs
  vectord mGPY;                       // Data values
	
  // Precomputed GP prediction operations
  covMatrix mInvR;                   // Inverse Correlation matrix

  size_t mMinIndex, mMaxIndex;
};

#endif
