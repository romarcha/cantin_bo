/**  \file criteria_combined.hpp \brief Combined criteria functions */
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

#ifndef  _CRITERIA_COMBINED_HPP_
#define  _CRITERIA_COMBINED_HPP_

#include <boost/numeric/ublas/vector_proxy.hpp>
#include "criteria_functors.hpp"


namespace bayesopt
{
  /**\addtogroup CriteriaFunctions
   * \brief Set of criterium functions to select the next point during
   * optimization/exploration
   */
  //@{

  /// Abstract class for combined criteria functions
  class CombinedCriteria: public Criteria
  {
  public:
    virtual ~CombinedCriteria() 
    {
      for(size_t i = 0; i<mCriteriaList.size(); ++i)
      {
	delete mCriteriaList[i];
      }
    };
    virtual int init(NonParametricProcess *proc, 
		     const std::vector<Criteria*>& list) 
    { 
      mProc = proc;
      mCriteriaList = list;
      return 0; 
    };

    int setParameters(const vectord &theta) 
    {
      using boost::numeric::ublas::subrange;
      vectori sizes(mCriteriaList.size());

      for (size_t i = 0; i < mCriteriaList.size(); ++i)
	{
	  sizes(i) = mCriteriaList[i]->nParameters();
	}

      if (theta.size() != norm_1(sizes))
	{
	  FILE_LOG(logERROR) << "Wrong number of criteria parameters"; 
	  return -1; 
	}

      size_t start = 0;
      for (size_t i = 0; i < mCriteriaList.size(); ++i)
	{
	  mCriteriaList[i]->setParameters(subrange(theta,start,start+sizes(i)));
	  start += sizes(i);
	}
      return 0;
    };

    size_t nParameters() 
    {
      size_t sum = 0;
      for (size_t i = 0; i < mCriteriaList.size(); ++i)
	{
	  sum += mCriteriaList[i]->nParameters();
	}
      return sum;
    };


  protected:
    NonParametricProcess* mProc;
    std::vector<Criteria*> mCriteriaList;
  };


  /// Wrapper class for linear combination of criterion functions.
  class SumCriteria: public CombinedCriteria
  {
  public:
    virtual ~SumCriteria() {};
  
    bool requireComparison(){ return false; };
    double operator()(const vectord &x)  
    {
      double sum = 0.0;
      for(size_t i = 0; i<mCriteriaList.size(); ++i)
	{ 
	  sum += (*mCriteriaList[i])(x); 
	}
      return sum;
    };

    std::string name() {return "cSum";};
  };

  /** Wrapper class for product of criterion functions. Typically one
   * being constant.
   */
  class ProdCriteria: public CombinedCriteria
  {
  public:
    virtual ~ProdCriteria() {};
  
    bool requireComparison(){ return false; };
    double operator()(const vectord &x)  
    {
      double prod = 1.0;
      for(size_t i = 0; i<mCriteriaList.size(); ++i)
	{ 
	  prod *= (*mCriteriaList[i])(x); 
	}
      return prod;
    };

    std::string name() {return "cProd";};
  };

  /**
   * \brief GP_Hedge model as describen in Hoffman et al. \cite Hoffman2011
   *
   * The coefficients of the bandit algorithm has been carefully selected
   * according to Shapire et al. Also, the implementation has been made to
   * avoid over or underflow.
   */
  class GP_Hedge: public CombinedCriteria
  {
  public:
    GP_Hedge();
    virtual ~GP_Hedge() {};
    int init(NonParametricProcess *proc, 
	     const std::vector<Criteria*>& list);

    bool requireComparison(){ return true; };
    double operator()(const vectord &x) { return (*mCurrentCriterium)(x); };

    void reset();
    bool checkIfBest(vectord& best, std::string& name,int& error_code);
    std::string name() {return "cHedge";};
  protected:
    int update_hedge();

    randEngine mtRandom;
    randFloat sampleUniform;
    vectord loss_, gain_, prob_, cumprob_;
    Criteria* mCurrentCriterium;
    std::vector<vectord> mBestLists;
    size_t mIndex;
 
  private:
    double computeLoss(const vectord& query)
    { return mProc->prediction(query)->getMean(); }
  };

  /**
   * \brief Modification of the GP_Hedge algorithm where the bandit gains are
   * random outcomes (Thompson sampling).
   */
  class GP_Hedge_Random: public GP_Hedge
  {
  public:
    virtual ~GP_Hedge_Random() {};
    std::string name() {return "cHedgeRandom";};
  private:
    double computeLoss(const vectord& query)
    { return mProc->prediction(query)->sample_query(mtRandom); }
  };


  //@}

} //namespace bayesopt


#endif
