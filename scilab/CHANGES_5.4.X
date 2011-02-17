            Changes between version 5.3.X and 5.4.0 of Scilab
            =================================================


New functions
=============

* The assert module provides a set of assertion functions.
  The goal of this module is to provide functions to check the 
  behavior of some other functions, for example in unit tests. 
  We emphasize the use of consistent tools for testing numerical 
  issues, with the goal of testing numerical algorithms more easily. 
  In particular, we provide a comparison function for two floating 
  point numbers, which allows to check that two numbers are 
  "numerically almost equal", i.e. that the relative error is small.
  This automatically fixes the bug #4381.

Bug Fixes:
==========

* bug 8608 - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified and optimized.
