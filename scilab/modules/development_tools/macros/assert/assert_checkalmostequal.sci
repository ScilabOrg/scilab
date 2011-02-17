// Copyright (C) 2008 - 2009 - INRIA - Michael Baudin
// Copyright (C) 2009 - 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkalmostequal ( varargin )
  //  Check that computed and expected are numerically close.
  //
  // Calling Sequence
  //   flag = assert_checkalmostequal ( computed , expected )
  //   flag = assert_checkalmostequal ( computed , expected , reltol )
  //   flag = assert_checkalmostequal ( computed , expected , reltol , abstol )
  //   flag = assert_checkalmostequal ( computed , expected , reltol , abstol , comptype )
  //   [flag,errmsg] = assert_checkalmostequal ( ... )
  //
  // Parameters
  //   computed: a 1x1 matrix of doubles, the computed result
  //   expected : a 1x1 matrix of doubles, the expected result
  //   reltol : a 1x1 matrix of doubles, the relative tolerance (default reltol=sqrt(%eps)).
  //   abstol : a 1x1 matrix of doubles, the absolute tolerance (default abstol=0).
  //   comptype : a 1x1 matrix of strings, "matrix" or "element" (default comptype="matrix"). The comparison type.
  //   flag : a 1x1 matrix of boolean, %t if computed is close to expected, %f if not
  //   errmsg : a 1x1 matrix of strings, the error message. If flag==%t, then errormsg=="". If flag==%f, then errmsg contains the error message.
  //
  // Description
  //   Performs silently if the two matrices of doubles or complex doubles 
  //   computed and expected are close.
  //   The variables computed and expected can be exchanged without changing the result.
  //
  //   Any optional input argument equal to the empty matrix is replaced by its default value.
  //
  //   We use the following algorithm.
  //   We compare first the real parts. In case of equality, we compare the imaginary parts.
  //
  //   The condition used is mixed relative and absolute:
  //   <programlisting>
  //     ( |e-c| <= reltol * max(|e|,|c|) + abstol ) 
  //   </programlisting>
  //   If comptype="matrix", the norm is used.
  //   If comptype="element", the absolute value are used and the two matrices are 
  //   almost equal if all the conditions are true. 
  //
  //   If the IEEE values %inf, -%inf or %nan values are in the matrices, 
  //   then they are almost equal only if the IEEE values are 
  //   at the same indices in the matrices.
  //
  //   The default comparison is based on a relative error, ensuring that 8 digits are common.
  //   This allows to assert the number of significant digits in the computed result.
  //
  //   This procedure only works when the computed and expected variables 
  //   are matrices of doubles. 
  //   It will generate an error in any other case.
  //
  //   If the comparison shows that computed is not almost equal to expected, 
  //   <itemizedlist>
  //   <listitem>if the errmsg output variable is not used, an error is generated,</listitem>
  //   <listitem>if the errmsg output variable is used, no error is generated.</listitem>
  //   </itemizedlist>
  //
  //   In the process of comparing the values, we separate %nan, +%inf, -%inf and remaining values.
  //   Comparing nan values between them is not possible. This is why we compare the 
  //   indices where %nan value occurs.
  //   If we form differences of infinity values, we produce %nan values.
  //   This is why we compare the indices where +%inf values occurs. 
  //   We do the same for -%inf values.
  //   Then, the non-nan, non-infinity values are actually compared.
  //
  //   The default value comptype="matrix" option performs the comparison for the matrices as a whole, 
  //   the norm of the difference of the matrices is used.
  //   The comptype="element" option performs the comparison elementwise, i.e. 
  //   all the elements of the matrices must be almost equal.
  //
  //   In general, the relative tolerance should be 
  //   set to a multiple of the machine precision %eps.
  //   The relative tolerance should also be chosen with the lowest 
  //   possible magnitude, that is, we shoud configure 
  //   the tolerance as accurately as possible.
  //   During the process of configuring the relative tolerance, we 
  //   suggest to use the following values, in that order :
  //   0 (all digits correct), %eps, 10*%eps, 100*%eps, 1.e3*%eps, 1.e4*%eps, ..., 
  //   1.e17*%eps (no digit correct).
  //   See below for examples of this.
  //
  // This function takes into account for complex numbers.
  // We first compare the real parts of the input arguments.
  // If this fails, we immediately return.
  // If this succeeds, we compare the imaginary parts of the input arguments.
  //
  // Examples
  //   // Comparisons which are successful.
  //   // Relative error :
  //   assert_checkalmostequal ( 1 , 1 ); 
  //   assert_checkalmostequal ( 1 , 1 , %eps ); 
  //   assert_checkalmostequal ( ones(10,1) , ones(10,1) , %eps );
  //   // Absolute error :
  //   assert_checkalmostequal ( 1.23456789123456789e-30 , 0 , 0 , 1.e-10 ); 
  //   assert_checkalmostequal ( [1 %nan], [1 %nan] , 0 , %eps );
  //
  //   // Comparisons which are failures.
  //   // Error message produced :
  //   assert_checkalmostequal ( 1 , 2 , %eps ); 
  //   // Error message produced :
  //   flag = assert_checkalmostequal ( 1 , 2 , %eps )
  //   // No error message produced :
  //   [flag,errmsg] = assert_checkalmostequal ( 1 , 2 , %eps )
  //   assert_checkalmostequal ( 1 , [2 3] , %eps );
  //   assert_checkalmostequal ( [%nan 1], [1 %nan] , %eps );
  //   assert_checkalmostequal ( 1 + 5 * %eps , 1 , %eps );
  //   assert_checkalmostequal ( 1.23456789123456789e-30 , 1.3e-30 , %eps );
  //
  //   // In the case where expected is nonzero, the 
  //   // tolerance for relative error should be a 
  //   // multiple of %eps.
  //   // The following test is a success and shows 
  //   // that less than 11 digits are lost with respect 
  //   // to the maximum possible accuracy.
  //   assert_checkalmostequal ( 1.23456 , 1.23457 , 1.e11*%eps );
  //
  //   // We cannot exchange the relative and absolute tolerances.
  //   // The following test pass: we use an absolute tolerance
  //   // because the expected value is zero.
  //   assert_checkalmostequal ( 1.23456789e-30 , 0 , 0 , 1.e-10 );
  //   // The following test fail: we use a relative tolerance.
  //   assert_checkalmostequal ( 0 , 1.23456789e-30 , 1.e-10 );
  //
  //   // We must configure the tolerance as tightly as possible.
  //   // The following test fails, because the tolerance is too small
  //   // with respect to the given numbers.
  //   assert_checkalmostequal ( 1.23456 , 1.23457 , %eps );
  //   // In order to get the number of common digits:
  //   assert_computedigits(1.23456 , 1.23457)
  //   // which returns 5.09...
  //   // We now make a judgment on the accuracy and conclude it is acceptable: 
  //   assert_checkalmostequal ( 1.23456 , 1.23457 , 1.e-5 );
  //
  //   // We give here a practical example of the use of assert_checkalmostequal.
  //   // We solve an averagely ill-conditionned system of linear
  //   // equations, based on the Hilbert matrix.
  //   n = 6;
  //   // The expected value is set by ourselves.
  //   expected = ones(n,1);
  //   A = testmatrix("hilb",n);
  //   // We compute the condition number of the matrix : ~10^8
  //   ceil(log10(cond(A)))
  //   // This means that the number of digits lost,  
  //   // predicted by theory, is 8.
  //   // The right-hand side is computed given A and expected.
  //   b = A * expected;
  //   // In this case, a Gauss algorithm with partial 
  //   // pivoting is used.
  //   computed = A\b;
  //   // The following test fails: we have lost some digits.
  //   assert_checkalmostequal(computed,expected,%eps)
  //   // We compute the actual number of common digits: from 10 to 12
  //   assert_computedigits(computed, expected)
  //   // We accept this computation.
  //   // The following test pass.
  //   assert_checkalmostequal(computed,expected,1.e5*%eps);
  //
  //   // The following test pass.
  //   assert_checkalmostequal ( [1 1.e5] , [2 1.e5] , 1.e-3 )
  //   // We see that 1 is not almost equal to 2: we must use the elementwise comparison.
  //   // The following test does not pass.
  //   assert_checkalmostequal ( [1 1.e5] , [2 1.e5] , 1.e-3 , [] , "element" )
  //
  //   // The following test pass.
  //   // It is non-trivial to take into account for IEEE values.
  //   [flag,errmsg] = assert_checkalmostequal ( [1.2345 %inf -%inf %nan] , [1.2346 %inf -%inf %nan] , 1.e-4 )
  //
  // // This function takes into account for complex numbers.
  // // The following test pass.
  // assert_checkalmostequal ( 1+%i , 1+(1+1.e-4)*%i , 1.e-3 , [], "element" );
  // // The following test fails.
  //  assert_checkalmostequal ( 1+%i , 1+(1+1.e-4)*%i , 1.e-5 , [], "element" );
  //
  // Authors
  //   Copyright (C) 2009 - 2010 - DIGITEO - Michael Baudin
  //   Copyright (C) 2008 - 2009 - INRIA - Michael Baudin

  [lhs,rhs]=argn()
  if ( and(rhs <> [2 3 4 5] ) ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d to %d are expected.") , "assert_checkalmostequal" , rhs , 2 , 5 )
    error(errmsg)
  end
  //
  // Get arguments
  computed = varargin(1)
  expected = varargin(2)
  reltol = argindefault ( rhs , varargin , 3 , sqrt(%eps) )
  abstol = argindefault ( rhs , varargin , 4 , 0 )
  comptype = argindefault ( rhs , varargin , 5 , "matrix" )
  //
  // Check types of variables
  if ( typeof(computed) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_checkalmostequal" , 1 , "computed" , typeof(computed) , "constant" )
    error(errmsg)
  end
  if ( typeof(expected) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_checkalmostequal" , 2 , "expected" , typeof(expected) , "constant" )
    error(errmsg)
  end
  if ( typeof(reltol) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_checkalmostequal" , 3 , "reltol" , typeof(reltol) , "constant" )
    error(errmsg)
  end  
  if ( typeof(abstol) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_checkalmostequal" , 4 , "abstol" , typeof(abstol) , "constant" )
    error(errmsg)
  end  
  if ( typeof(comptype) <> "string" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_checkalmostequal" , 5 , "comptype" , typeof(comptype) , "string" )
    error(errmsg)
  end  
  //
  // Check sizes of variables
  if ( size(reltol,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkalmostequal" , 3 , "reltol" , size(reltol,"*") , 1 )
    error(errmsg)
  end
  if ( size(abstol,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkalmostequal" , 4 , "abstol" , size(abstol,"*") , 1 )
    error(errmsg)
  end
  if ( size(comptype,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkalmostequal" , 5 , "comptype" , size(comptype,"*") , 1 )
    error(errmsg)
  end
  //
  // Check values of variables
  if ( reltol < 0 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_checkalmostequal" , 3 , "reltol" )
    error(errmsg)
  end
  if ( abstol < 0 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_checkalmostequal" , 4 , "abstol" )
    error(errmsg)
  end
  if ( and ( comptype <> ["matrix" "element"] ) ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has value %s.") , "assert_checkalmostequal" , 5 , "comptype" , comptype )
    error(errmsg)
  end
  //
  // Proceed...
  ncom = size(computed)
  nexp = size(expected)
  if ( or(ncom <> nexp) ) then
    cstr = "[" + strcat(string(ncom)," ") + "]"
    estr = "[" + strcat(string(nexp)," ") + "]"
    errmsg = sprintf ( gettext ( "%s: We have size(computed) = %s while size(expected) = %s: not consistent comparison.") , "assert_checkalmostequal" , cstr , estr )
    error(errmsg)
  end  
  //
  areequal_re = assert_arealmostequal ( real(computed) , real(expected) , reltol , abstol , comptype )
  if ( areequal_re ) then
    areequal = assert_arealmostequal ( imag(computed) , imag(expected) , reltol , abstol , comptype )
  else
    areequal = %f
  end
  //
  if ( areequal ) then
    flag = %t
    errmsg = ""
  else
    flag = %f
    // Change the message if the matrix contains more than one value
    if ( size(expected,"*") == 1 ) then
      estr = string(expected)
    else
      estr = "[" + string(expected(1)) + " ...]"
    end
    if ( size(computed,"*") == 1 ) then
      cstr = string(computed)
    else
      cstr = "[" + string(computed(1)) + " ...]"
    end
    relstr = string(reltol)
    absstr = string(abstol)
    errmsg = msprintf(gettext("%s: Assertion failed: expected = %s while computed = %s with reltol=%s, abstol=%s"), ..
      "assert_checkalmostequal",estr,cstr,relstr,absstr)
    if ( lhs < 2 ) then
      // If no output variable is given, generate an error
      assert_generror ( errmsg )
    end
  end
endfunction

// Returns the indices where 
//  * kpinf : x(kpinf) == +%inf, 
//  * kninf : x(kninf) == -%inf, 
//  * knan : x(knan) is a %nan 
//  * kreg : x(kreg) is not an infinity, not a nan
//  * xreg = x(kreg)
// These 4 sets of indices have no intersection.
//
// Example :
// x = [1 2 3 -%inf %inf %nan %inf %nan -%inf 4 5 6]
// [kpinf , kninf , knan , kreg , xreg] = infnanindices ( x )
// xreg = [1 2 3 4 5 6]
// kreg = [1 2 3 10 11 12]
// knan = [6 8]
// kninf = [4 9]
// kpinf = [5 7]
function [kpinf , kninf , knan , kreg , xreg] = infnanindices ( x )
  kpinf = find(x==%inf)
  kninf = find(x==-%inf)
  knan = find(isnan(x))
  kreg = find(abs(x)<>%inf & ~isnan(x))
  xreg = x(kreg)
endfunction


function areequal = assert_arealmostequal ( computed , expected , reltol , abstol , comptype )
  //
  // Decompose the expected value into nan indices, inf indices and regular indices
  // This allows to solve the following issue:
  // if computed is %inf and expected is %inf, the difference is %nan,
  // which makes the computations fail.	
  [kcpinf , kcninf , kcnan , kcreg , creg] = infnanindices ( computed )
  [kepinf , keninf , kenan , kereg , ereg] = infnanindices ( expected )
  //
  if ( comptype == "matrix" ) then
    areclose = ( norm ( creg - ereg ) <= reltol * max(norm(ereg),norm(creg) ) + abstol )
  else
    entries = ( abs(creg-ereg) <= reltol * max(abs(ereg),abs(creg)) + abstol )
    // Compute the global condition from the entries conditions
    areclose = and(entries)
  end
  // The regular values must be almost equal and 
  // * the +%inf must be at the same place,
  // * the -%inf must be at the same place,
  // * the %nan must be at the same place.
  areequal = ( areclose & and(kcpinf == kepinf) & and(kcninf == keninf) & and(kcnan == kenan) )
endfunction
function argin = argindefault ( rhs , vararglist , ivar , default )
  // Returns the value of the input argument #ivar.
  // If this argument was not provided, or was equal to the 
  // empty matrix, returns the default value.
  if ( rhs < ivar ) then
    argin = default
  else
    if ( vararglist(ivar) <> [] ) then
      argin = vararglist(ivar)
    else
      argin = default
    end
  end
endfunction

