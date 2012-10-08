/** -*- c++ -*- \file ublas_extra.hpp \brief Extra functions for Ublas library */
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

#ifndef __UBLAS_EXTRA_HPP__
#define __UBLAS_EXTRA_HPP__

template<class V, class D>
int append(V& vector, D element)
{
  assert(typeid(v::value_type) == typeid(D));

  // This method is super inefficient but there seems to be the uBlas style.
  size_t size = vector.size();
  vector.resize(size+1,true);
  vector(size) = element;
  return 0;
};

#endif
