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

#include <cstdio>

#include "nonparametricprocess.hpp"
#include "cholesky.hpp"
#include "ublas_extra.hpp"

NonParametricProcess::NonParametricProcess(double noise):
  InnerOptimization(),  mKernel(NULL), mMean(NULL), mRegularizer(noise)
{ 
  mMinIndex = 0; 
  mMaxIndex = 0;   
  setAlgorithm(BOBYQA);
  setLimits(0.,100.);
}


NonParametricProcess::~NonParametricProcess()
{
  if (mKernel != NULL)
    delete mKernel;

  if (mMean != NULL)
    delete mMean;
}

int NonParametricProcess::setKernel (const vectord &thetav,
				     kernel_name k_name)
{
  if (mKernel != NULL)
    delete mKernel;

  switch(k_name)
    {
    case K_MATERN_ISO1: mKernel = new MaternIso1(); break;
    case K_MATERN_ISO3: mKernel = new MaternIso3(); break;
    case K_MATERN_ISO5: mKernel = new MaternIso5(); break;
    case K_SE_ISO: mKernel = new SEIso(); break;
    case K_SE_ARD: mKernel = new SEArd(); break;
    default:
      std::cout << "Error: kernel function not supported." << std::endl;
      return -1;
    }

  mKernel->setScale(thetav);

  return 0;
}


int NonParametricProcess::setMean (const vectord &muv,
				   mean_name m_name)
{
  if (mMean != NULL)
    delete mMean;

  switch(m_name)
    {    
    case M_ZERO: mMean = new ZeroFunction(); break;
    case M_ONE: mMean = new OneFunction(); break;
    case M_CONSTANT: mMean = new ConstantFunction(); break;
    case M_LINEAR: mMean = new LinearFunction(); break;
    case M_LINEAR_CONSTANT: mMean = new LinearPlusConstantFunction(); break;
    default:
      std::cout << "Error: mean function not supported." << std::endl;
      return -1;
    }

  mMean->setParameters(muv);

  return 0;
}


int NonParametricProcess::fitGP()
{
  vectord optimalTheta = mKernel->getScale();
  int error = -1;

  std::cout << "Initial theta: " << optimalTheta << std::endl;
  innerOptimize(optimalTheta);
  mKernel->setScale(optimalTheta);
  std::cout << "Final theta: " << optimalTheta << std::endl;

  //#ifdef USE_CHOLESKY
  error = computeCholeskyCorrelation();
  //#else
  error = computeInverseCorrelation();
  //#end

  if (error < 0)   
    return error;

  return precomputePrediction();
} // fitGP

int NonParametricProcess::addNewPointToGP( const vectord &Xnew,
					   double Ynew)
{
  assert( mGPXX[1].size() == Xnew.size() );

  vectord newK = computeCrossCorrelation(Xnew);
  double selfCorrelation = (*mKernel)(Xnew, Xnew) + mRegularizer;
  
  addSample(Xnew,Ynew);

  //TODO: Choose one!
  addNewPointToCholesky(newK,selfCorrelation);
  addNewPointToInverse(newK,selfCorrelation);

  // USED FOR TESTING
  // using boost::numeric::ublas::identity_matrix;
  // using boost::numeric::ublas::lower;
  // matrixd testInv(identity_matrix<double>(mInvR.size1()));
  // cholesky_solve(mL,testInv,lower());
  // matrixd newR = mInvR;
  // matrixd newL = mL;

  // computeCholeskyCorrelation();

  // matrixd testInv2(identity_matrix<double>(mInvR.size1()));
  // cholesky_solve(mL,testInv2,lower());

  // computeInverseCorrelation();

  // for(size_t i=0; i<mInvR.size1(); ++i)
  //   for(size_t j=0; j<mInvR.size2(); ++j)
  //     if (std::abs(mL(i,j)-newL(i,j)) > 0.01)
  // 	{
  // 	  std::cout << "ERROR! " << i << "," << j << " | "
  // 		    << mL(i,j) << "," << newL(i,j) << std::endl;
  // 	  std::getchar();
  // 	}
  
  return precomputePrediction();
} // addNewPointToGP


int NonParametricProcess::addNewPointToCholesky(const vectord& correlation,
						double selfcorrelation)
{
  //TODO: Optimize
  vectord newK = correlation;
  append(newK, selfcorrelation);
  cholesky_add_row(mL,newK);

  return 1;
}


int NonParametricProcess::computeCholeskyCorrelation()
{
  size_t nSamples = mGPXX.size();
  mL.resize(nSamples,nSamples);
  
  const matrixd K = computeCorrMatrix();
  return cholesky_decompose(K,mL);
}

int NonParametricProcess::addNewPointToInverse(const vectord& correlation,
					       double selfcorrelation)
{
  size_t nSamples = correlation.size();
  
  vectord Li(nSamples);
  vectord wInvR(nSamples);
  double wInvRw;
  double Ni;

  noalias(wInvR) = prod(correlation,mInvR);
  wInvRw = inner_prod(wInvR,correlation);

  Ni = 1/(selfcorrelation - wInvRw);

  noalias(Li) = -Ni * wInvR;
  mInvR += outer_prod(Li,Li) / Ni;
  
  //TODO: There must be a better way to do this.
  mInvR.resize(nSamples+1,nSamples+1,true);
  
  Li.resize(nSamples+1);
  Li(nSamples) = Ni;
  
  row(mInvR,nSamples) = Li;
  column(mInvR,nSamples) = Li;

  return 1;

}


int NonParametricProcess::computeInverseCorrelation()
{
  size_t nSamples = mGPXX.size();
  if ( (nSamples != mInvR.size1()) || (nSamples != mInvR.size2()) )
    mInvR.resize(nSamples,nSamples);
    
  matrixd corrMatrix = computeCorrMatrix();

  //return InvertMatrix(corrMatrix,mInvR);
  return inverse_cholesky(corrMatrix,mInvR);
}




matrixd NonParametricProcess::computeCorrMatrix(int dth_index)
{
  size_t nSamples = mGPXX.size();
  matrixd corrMatrix(nSamples,nSamples);
  
  for (size_t ii=0; ii< nSamples; ++ii)
    {
      for (size_t jj=0; jj < ii; ++jj)
	{
	  corrMatrix(ii,jj) = (*mKernel)(mGPXX[ii], mGPXX[jj], 
						  dth_index);
	  corrMatrix(jj,ii) = corrMatrix(ii,jj);
	}
      corrMatrix(ii,ii) = (*mKernel)(mGPXX[ii],mGPXX[ii], dth_index);
      if (dth_index < 0) 
	corrMatrix(ii,ii) += mRegularizer;
    }
  return corrMatrix;
}



vectord NonParametricProcess::computeCrossCorrelation(const vectord &query)
{
  vectord knx(mGPXX.size());

  //TODO: Replace by transform
  for (size_t ii=0; ii<mGPXX.size(); ++ii)
    knx(ii) = (*mKernel)(mGPXX[ii], query);

  return knx;
}
