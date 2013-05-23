#!/usr/bin/env python
# -------------------------------------------------------------------------
#    This file is part of BayesOpt, an efficient C++ library for 
#    Bayesian optimization.
#
#    Copyright (C) 2011-2013 Ruben Martinez-Cantin <rmcantin@unizar.es>
# 
#    BayesOpt is free software: you can redistribute it and/or modify it 
#    under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    BayesOpt is distributed in the hope that it will be useful, but 
#    WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with BayesOpt.  If not, see <http://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------

import numpy as np
from bayesoptmodule import BayesOptContinuous
from multiprocessing import Process, Pipe

# Function for testing.
def testfunc(Xin):
    total = 5.0
    for value in Xin:
        total = total + (value -0.33)*(value-0.33)
    return total


def worker(pipe):
    x = None
    while True:
        x = pipe.recv()
        if x == 'STOP':
            break

        result = testfunc(x)
        pipe.send(result)


class BayesOptProcess(Process,BayesOptContinuous):

    def __init__ (self, pipe):
        Process.__init__(self)
        BayesOptContinuous.__init__(self)
        self.pipe = pipe

    def run(self):
        mvalue, x_out, error = self.optimize()
        self.pipe.send('STOP')
        self.mvalue = mvalue
        self.x_out = x_out
        self.error = error

        return

    def evalfunc(self, x):
        self.pipe.send(x)
        result = self.pipe.recv()
        return result


if __name__ == '__main__':
    pipe_par, pipe_child = Pipe()

    bo = BayesOptProcess(pipe_child)
    bo.params['n_iterations'] = 50
    bo.params['n_init_samples'] = 20
    bo.params['s_name'] = "sGaussianProcessNormal"
    bo.params['c_name'] = "cHedge(cEI,cLCB,cExpReturn,cOptimisticSampling)"

    p = Process(target=worker, args=(pipe_par,))

    bo.start()
    p.start()
    
    bo.join()
    p.join()
