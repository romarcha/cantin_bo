
#include "gaussian_process_ign.hpp"
#include "cholesky.hpp"
#include "trace_ublas.hpp"

using boost::numeric::ublas::inplace_solve;
using boost::numeric::ublas::lower_tag;
using boost::numeric::ublas::lower;
  

GaussianProcessIGN::GaussianProcessIGN( double noise, double alpha, 
					double beta, double delta):
  GaussianProcess(noise),
  mAlpha(alpha), mBeta (beta), mDelta2(delta)
{}  // Constructor



GaussianProcessIGN::~GaussianProcessIGN()
{} // Default destructor




double GaussianProcessIGN::negativeLogLikelihood(size_t index)
{
  matrixd K = computeCorrMatrix();
  size_t n = K.size1();
  matrixd L(n,n);
  cholesky_decompose(K,L);

  vectord alphU(mMeanV);
  inplace_solve(L,alphU,lower_tag());
  double eta = inner_prod(alphU,alphU) + 1/mDelta2;
  
  vectord alphY(mGPY);
  inplace_solve(L,alphY,lower_tag());
  double mu     = inner_prod(mMeanV,alphY) / eta;
  double YInvRY = inner_prod(mGPY,alphY);
    
  double sigma = (mBeta + YInvRY - mu*mu*eta) / (mAlpha + (n+1) + 2);
  
  svectord colMu(n,mu);
  vectord yumu = mGPY - colMu;
  
  alphY = yumu;
  inplace_solve(L,alphY,lower_tag());

  double lik1 = inner_prod(yumu,alphY) / (2*sigma); 
  double lik2 = trace(L) + 0.5*n*log(sigma) + n*0.91893853320467; //log(2*pi)/2

  double th = mKernel->getScale(index);

  return lik1 + lik2 + mBeta/2 * th -
    (mAlpha+1) * log(th);
}


int GaussianProcessIGN::prediction( const vectord &query,
				    double& yPred, double& sPred)
{
  size_t n = mGPXX.size();
  //vectord rInvR(n);
  double kn;
  double uInvRr, rInvRr, rInvRy;
  double meanf = mMean->getMean(query);

  vectord colR = computeCrossCorrelation(query);
  kn = (*mKernel)(query, query);

#if USE_CHOL
  vectord invRr(colR);
  inplace_solve(mL,invRr,lower_tag());
  rInvRr = inner_prod(invRr,invRr);
  uInvRr = inner_prod(mUInvR, invRr);  
  rInvRy = inner_prod(invRr, mInvRy );
#else
  vectord rInvR = prod(colR,mInvR);
	
  rInvRr = inner_prod(rInvR,colR);
  uInvRr = inner_prod(mUInvR,colR);
  rInvRy = inner_prod(colR,mInvRy);
#endif
  
  yPred = meanf*mMu + rInvRy;
  sPred = sqrt( mSig * (kn - rInvRr + (meanf - uInvRr) * (meanf - uInvRr) 
			/ mUInvRUDelta ) );

  return 1;
}
	

int GaussianProcessIGN::precomputePrediction()
{
  size_t nSamples = mGPXX.size();

  mUInvR.resize(nSamples,false);
  mInvRy.resize(nSamples,false);

#if USE_CHOL
  mAlphaV.resize(nSamples,false);
  mAlphaV = mGPY;
  cholesky_solve(mL,mAlphaV,lower());

  //  vectord alphaMean(mMeanV);
  mUInvR = mMeanV;
  inplace_solve(mL,mUInvR,lower_tag());
  mUInvRUDelta = inner_prod(mUInvR,mUInvR) + 1/mDelta2;

  mMu = inner_prod(mMeanV,mAlphaV) / mUInvRUDelta;
  double YInvRY = inner_prod(mGPY,mAlphaV);

  mInvRy = mGPY - mMeanV*mMu;
  inplace_solve(mL,mInvRy,lower_tag());
#else
  // mInvR.resize(nSamples,nSamples);
  // mInvR.assign(boost::numeric::ublas::identity_matrix<double>(nSamples));
  // cholesky_solve(mL,mInvR,lower());
  mUInvR = prod(mMeanV,mInvR);
  mUInvRUDelta = inner_prod(mUInvR,mMeanV) + 1/mDelta2;
  
  vectord YInvR(nSamples);
  double YInvRY;
  
  mMu =  inner_prod(mUInvR,mGPY) / mUInvRUDelta;
  
  noalias(YInvR) = prod(mGPY,mInvR);
  YInvRY = inner_prod(YInvR,mGPY);
  vectord ymu = mGPY - mMeanV*mMu;
  mInvRy = prod(mInvR,ymu);
#endif  

  mSig = (mBeta + YInvRY - mMu*mMu*mUInvRUDelta) / (mAlpha + (nSamples+1) + 2);
  
  return 1;
}


