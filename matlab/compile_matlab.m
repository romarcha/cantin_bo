% You can also change ../lib for the correspoding install path
% MATLAB
mex -output bayesopt bayesoptmex.c ../lib/bayesopt.so ../lib/libnlopt.so ...
   -I../include -I../wrappers -I../nlopt/api 