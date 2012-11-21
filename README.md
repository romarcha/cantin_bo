BayesOpt
========

This is an efficient, C++ implementation of several Bayesian
optimization algorithms. See References for some of the papers.

It combines the use of a stochastic process as a surrogate function
with some "active learning" criterion to find the optimum of an
"arbitrary" function using very few iterations. It can also be used
for sequential experimental design and stochastic bandits by selecting
the adequate criterion.

Using old C++ standards (C++98), it can be used with many compilers in
Windows, Linux, Mac OS. There are APIs for C/C++, Python and
Matlab/Octave.

Important: This code is free to use. However, if you are using, or
plan to use, the library, specially if it is for research or academic
purposes, please send me an email (rmcantin@unizar.es) with your name,
institution and a brief description of your interest for this code
(one or two lines).

----------------------------------------------------------------------
For instructions about how to install and use the library, please
visit:

https://bitbucket.org/rmcantin/bayesian-optimization/wiki/Home

----------------------------------------------------------------------
If you use BayesOpt in work that leads to a publication, we would
appreciate it if you would kindly cite BayesOpt in your
manuscript. Cite BayesOpt as something like:

Ruben Martinez-Cantin, BayesOpt: a toolbox for nonlinear-optimization,
experimental design and stochastic bandits,
http://bitbucket.org/rmcantin/bayesian-optimization

----------------------------------------------------------------------
Copyright (C) 2011-2012 Ruben Martinez-Cantin <rmcantin@unizar.es>

BayesOpt is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BayesOpti is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with BayesOpt. If not, see <http:www.gnu.org/licenses/>.
----------------------------------------------------------------------