/** \file optimization.hpp 
    \brief Abstract class for optimization algorithms */
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

#ifndef __OPTIMIZATION_HPP__
#define __OPTIMIZATION_HPP__

#include "specialtypes.hpp"

namespace bayesopt {

  class Optimization
  {
  public:
    Optimization(){};
    virtual ~Optimization(){};

    virtual int run(vectord& result) = 0;
    virtual double evaluate(const vectord& query) = 0;
  };

}


#endif
