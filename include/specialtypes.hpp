#ifndef __SPECIALTYPES_HPP__
#define __SPECIALTYPES_HPP__

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

#include "defaults.h"

typedef boost::numeric::ublas::vector<double>                   vectord;
typedef boost::numeric::ublas::zero_vector<double>             zvectord;
typedef boost::numeric::ublas::scalar_vector<double>           svectord;
typedef boost::numeric::ublas::matrix<double>                   matrixd;
typedef boost::numeric::ublas::identity_matrix<double>             eyed;

typedef std::vector<vectord>                                   vecOfvec;

// Surprisingly, this is the most efficient version of a growing matrix 
// for uBlas, but I leave here the old trial because it might change in
// future.
typedef boost::numeric::ublas::matrix<double>                 covMatrix;

// typedef boost::numeric::ublas::bounded_matrix<double, MAX_ITERATIONS, 
//                                            MAX_ITERATIONS> covMatrix; 

// typedef boost::numeric::ublas::symmetric_matrix<double,lower,row_major,
//                          bounded_array<double,90000> >       covMatrix;

#endif
