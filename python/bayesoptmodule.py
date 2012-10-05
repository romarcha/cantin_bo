#!/usr/bin/env python
import numpy as np
import bayesopt as kp

class BayesOptModule:
    def __init__(self):
        # Let's define the parameters
        # For different options: see ctypes.h and cpp
        # If a parameter is not define, it will be automatically set
        # to a default value.
        self.params = kp.initialize_params()
        self.n = 5                      # n dimensions
        self.lb = np.zeros((self.n,))
        self.ub = np.ones((self.n,))

    # Function for testing
    def evalfunc(self,Xin):
        total = 10.0
        for value in Xin:
            total = total + (value -0.53)*(value-0.53)
        return total

    def optimize(self):
        mvalue, x_out, error = kp.optimize(self.evalfunc, self.n,
                                           self.lb, self.ub,self.params)
        
        return mvalue, x_out, error



if __name__ == "__main__":
    bopt = BayesOptModule()
    mvalue, x_out, error = bopt.optimize()
    print "Result", x_out
