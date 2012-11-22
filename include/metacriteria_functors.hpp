/** -*- c++ -*- \file metacriteria_functors.hpp 
    \brief Functions to combine different criteria */
/*
-------------------------------------------------------------------------
   This file is part of BayesOpt, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011-2012 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
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

#ifndef  _METACRITERIA_FUNCTORS_HPP_
#define  _METACRITERIA_FUNCTORS_HPP_

#include <boost/scoped_ptr.hpp>
#include "criteria_functors.hpp"

/**\addtogroup MetaCriteriaFunctions
 * \brief Functions that combine different criteria in a single step.
 */
//@{


/**
 * \brief Abstract class for Metacriteria interface.
 * The Metacriteria can be used to combine different criteria.
 */
class MetaCriteria
{
public:
  MetaCriteria(NonParametricProcess* proc): mProc(proc) {};
  virtual ~MetaCriteria() {};
  virtual int initializeSearch() = 0;
  virtual double operator()( const vectord &x) = 0;
  virtual bool checkIfBest(vectord& xNext,
			   criterium_name& name,
			   int& error_code) = 0;
  
  static MetaCriteria* create(criterium_name name,
			      NonParametricProcess* proc);

protected:
  NonParametricProcess* mProc;
};


////////////////////////////////////////////////////////////////////////////
/**
 * \brief Abstract class for Metacriteria interface.
 * The Metacriteria can be used to combine different criteria.
 */
class SingleCriteria: public MetaCriteria
{
public:
  SingleCriteria(criterium_name name, NonParametricProcess* proc);
  virtual ~SingleCriteria() {};
  
  double operator()( const vectord &x)
  { return (*mCriterium)(x); };

  int initializeSearch() { return 1;};
  bool checkIfBest(vectord& xNext, criterium_name& name, int& error_code)
  { 
    error_code = 0;  
    return true; 
  };

private:
  boost::scoped_ptr<Criteria> mCriterium;
};



/**
 * \brief GP_Hedge model as describen in Hoffman et al. \cite Hoffman2011
 *
 * The coefficients of the bandit algorithm has been carefully selected
 * according to Shapire et al. Also, the implementation has been made to
 * avoid over or underflow.
 */
class GP_Hedge: public MetaCriteria
{
public:
  GP_Hedge(NonParametricProcess *proc);
  virtual ~GP_Hedge();
  double operator()( const vectord &x)
  { return (*mCurrentCriterium)(x); };

  int update_hedge();
  int initializeSearch()
  {
    mIndex = 0;
    mCurrentCriterium = mCriteriaList[mIndex];
    mBestLists.clear();
    return 1;
  };

  bool checkIfBest(vectord& best,criterium_name& name,int& error_code);


protected:
  randEngine mtRandom;
  randFloat sampleUniform;
  vectord loss_, gain_, prob_, cumprob_;
  std::vector<Criteria*> mCriteriaList;
  Criteria* mCurrentCriterium;
  std::vector<vectord> mBestLists;
  size_t mIndex;

 
private:
  double computeLoss(const vectord& query)
  {	
    return mProc->prediction(query)->getMean();
  }
};

/**
 * \brief Modification of the GP_Hedge algorithm where the bandit gains are
 * random outcomes.
 */
class GP_Hedge_Random: public GP_Hedge
{
public:
  GP_Hedge_Random(NonParametricProcess *proc):
    GP_Hedge(proc) {};

  virtual ~GP_Hedge_Random() {};

private:
  double computeLoss(const vectord& query)
  { 
    return mProc->prediction(query)->sample_query(mtRandom);
  }
};

//@}

#endif
