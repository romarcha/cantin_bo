%-----------------------------------------------------------------------------
%   This file is part of BayesOptimization, an efficient C++ library for 
%   Bayesian optimization.
%
%   Copyright (C) 2011 Ruben Martinez-Cantin <rmcantin@unizar.es>
% 
%   BayesOptimization is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 3 of the License, or
%   (at your option) any later version.
%
%   BayesOptimization is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with BayesOptimization.  If not, see <http://www.gnu.org/licenses/>.
%-----------------------------------------------------------------------------
clear all, close all
addpath('testfunctions')

params.iterations = 200;
params.criteria = 'ei';
params.surrogate = 'gp';
params.kernel = 'materniso3';

lb = ones(2,1)*0;
ub = ones(2,1)*3.2;

tic;
bayesopt('michalewicz',2,params,lb,ub)
toc;
