// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function order = assert_comparecomplex ( varargin )
  // Compare complex numbers with a tolerance.
  // 
  // Calling Sequence
  //   order = assert_comparecomplex ( a , b )
  //   order = assert_comparecomplex ( a , b , reltol )
  //   order = assert_comparecomplex ( a , b , reltol , abstol )
  // 
  // Parameters
  //   a : a 1x1 matrix of doubles, the first value to be compared
  //   b : a 1x1 matrix of doubles, the second value to be compared
  //   reltol : a 1x1 matrix of doubles, the relative tolerance (default reltol=sqrt(%eps)).
  //   abstol : a 1x1 matrix of doubles, the absolute tolerance (default abstol=0).
  //   order : a 1x1 matrix of floating point integers, the order. Returns order=0 is a is almost equal to b, order=-1 if a < b, order=+1 if a > b.
  // 
  // Description
  // Compare first by real parts, then by imaginary parts.
  // Takes into account numerical accuracy issues, by using 
  // a mixed relative and absolute tolerance criteria.
  //
  // Any optional input argument equal to the empty matrix is replaced by its default value.
  //
  // We use the following algorithm.
  //
  // We compare first the real parts. In case of tie, we compare the imaginary parts.
  //
  // We process the IEEE values and choose the order : -%inf < 0 < %inf < %nan.
  // If none of the values is special, we use the condition :
  //   <programlisting>
  //     cond = ( abs(a-b) <= reltol * max(abs(a),abs(b)) + abstol ) 
  //   </programlisting>
  //
  // This algorithm is designed to be used into sorting 
  // algorithms.
  // It allows to take into account for the portability issues
  // related to the outputs of functions producing 
  // matrix of complex doubles.
  // If this algorithm is plugged into a sorting function, 
  // it allows to consistently produce a sorted matrix, 
  // where the order can be independent of the operating system,
  // the compiler or other forms of issues modifying the 
  // order (but not the values).
  // 
  // Examples
  //   // Compare real values
  //   assert_comparecomplex ( 1 , -1 ) // 1
  //   assert_comparecomplex ( -1 , 1 ) // -1
  //   assert_comparecomplex ( 1 , 1 ) // 0
  //
  //   // Compare complex values #1
  //   assert_comparecomplex ( 1+2*%i , 1+3*%i ) // -1
  //   assert_comparecomplex ( 1+3*%i , 1+2*%i ) // 1
  //   assert_comparecomplex ( 1+2*%i , 1+2*%i ) // 0
  //
  //   // Compare complex values #2
  //   assert_comparecomplex ( 1+%i , -1+%i ) // 1
  //   assert_comparecomplex ( -1+%i , 1+%i ) // -1
  //   assert_comparecomplex ( 1+%i , 1+%i ) // 0
  //   [order,msg] = assert_comparecomplex ( 1+%i , 1+%i )
  //
  //   // Compare with tolerances : equality cases
  //   assert_comparecomplex ( 1.2345+%i , 1.2346+%i , %eps , 1.e-3 ) // 0
  //   assert_comparecomplex ( 1.2345+%i , 1.2346+%i , 1.e12*%eps , 0 ) // 0
  //   assert_comparecomplex ( 1+1.2345*%i , 1+1.2347*%i , %eps , 1.e-3 ) // 0
  //   assert_comparecomplex ( 1+1.2345*%i , 1+1.2347*%i , 1.e12*%eps , 0 ) // 0
  //
  //   // Compare more realistic data
  //  x = [
  //    -0.123452 - 0.123454 * %i
  //    -0.123451 + 0.123453 * %i
  //     0.123458 - 0.123459 * %i
  //     0.123456 + 0.123457 * %i
  //  ];
  //  // Consider less than 4 significant digits
  //  for i = 1 : size(x,"*")-1
  //    order = assert_comparecomplex ( x(i) , x(i+1) , 1.e-4 );
  //    mprintf("compare(x(%d),x(%d))=%d\n",i,i+1,order)
  //  end
  //
  // // Compare data from bug #415
  //  x = [
  //    -1.9914145
  //    -1.895889
  //    -1.6923826
  //    -1.4815461
  //    -1.1302576
  //    -0.5652256 - 0.0655080 * %i
  //    -0.5652256 + 0.0655080 * %i
  //    0.3354023 - 0.1602902 * %i
  //    0.3354023 + 0.1602902 * %i
  //     1.3468911
  //     1.5040136
  //     1.846668
  //     1.9736772
  //     1.9798866
  //  ];
  //  // Consider less than 4 significant digits
  //  for i = 1 : size(x,"*")-1
  //    order = assert_comparecomplex ( x(i) , x(i+1) , 1.e-5 );
  //    mprintf("compare(x(%d),x(%d))=%d\n",i,i+1,order)
  //  end
  //
  // Authors
  // Michael Baudin, DIGITEO, 2009-2010
  //
  // Bibliography
  // http://gitweb.scilab.org/?p=scilab.git;a=blob;f=scilab/modules/polynomials/tests/nonreg_tests/bug_415.tst;h=0c716a3bed0dfb72c831972d19dbb0814dffde2b;hb=HEAD
  // http://gitweb.scilab.org/?p=scilab.git;a=blob_plain;f=scilab/modules/cacsd/tests/nonreg_tests/bug_68.tst;h=920d091d089b61bf961ea9e888b4d7d469942a14;hb=4ce3d4109dd752fce5f763be71ea639e09a12630

  [lhs,rhs]=argn()
  if ( and(rhs <> [2 3 4] ) ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d to %d are expected.") , "assert_comparecomplex" , rhs , 2 , 4 )
    error(errmsg)
  end
  //
  // Get arguments
  a = varargin(1)
  b = varargin(2)
  reltol = argindefault ( rhs , varargin , 3 , sqrt(%eps) )
  abstol = argindefault ( rhs , varargin , 4 , 0 )
  //
  // Check types of variables
  if ( typeof(a) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_comparecomplex" , 1 , "a" , typeof(a) , "constant" )
    error(errmsg)
  end
  if ( typeof(a) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_comparecomplex" , 2 , "b" , typeof(b) , "constant" )
    error(errmsg)
  end
  if ( typeof(reltol) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_comparecomplex" , 3 , "reltol" , typeof(reltol) , "constant" )
    error(errmsg)
  end  
  if ( typeof(abstol) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_comparecomplex" , 4 , "abstol" , typeof(abstol) , "constant" )
    error(errmsg)
  end  
  //
  // Check sizes of variables
  if ( size(a,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_comparecomplex" , 1 , "a" , size(a,"*") , 1 )
    error(errmsg)
  end
  if ( size(b,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_comparecomplex" , 2 , "b" , size(b,"*") , 1 )
    error(errmsg)
  end
  if ( size(reltol,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_comparecomplex" , 3 , "reltol" , size(reltol,"*") , 1 )
    error(errmsg)
  end
  if ( size(abstol,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_comparecomplex" , 4 , "abstol" , size(abstol,"*") , 1 )
    error(errmsg)
  end
  //
  // Check values of variables
  if ( reltol < 0 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_comparecomplex" , 3 , "reltol" )
    error(errmsg)
  end
  if ( abstol < 0 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_comparecomplex" , 4 , "abstol" )
    error(errmsg)
  end
  //
  ord_re = assert_compdata ( real(a) , real(b) , reltol , abstol )
  if ( ord_re == 0 ) then
    // Tie on the real part: compare imaginary parts
    ord_im = assert_compdata ( imag(a) , imag(b) , reltol , abstol )
    if ( ord_im == 0 ) then
      // Tie on imaginary parts too: two numbers are "equal"
      order = 0
    elseif ( ord_im == -1 ) then
      order = -1
    else
      order = 1
    end
  elseif ( ord_re == -1 ) then
    order = -1
  else
    order = 1
  end
endfunction

function order = assert_compdata ( a , b , reltol , abstol )
  if ( a == %inf ) then
    if ( isnan(b) ) then
      order = -1
    elseif ( b == %inf ) then
      order = 0
    else
      order = 1
    end
  elseif ( a == -%inf ) then
    if ( b == -%inf ) then
      order = 0
    else
      order = -1
    end
  elseif ( isnan(a) ) then
    if ( isnan(b) ) then
      order = 0
    else
      order = 1
    end
  else
    if ( isnan(b) ) then
      order = -1
    elseif ( b == -%inf ) then
      order = 1
    elseif ( b == %inf ) then
      order = -1
    else
      areequal = abs ( a - b ) <= reltol * max ( abs(a) , abs(b) ) + abstol
      if ( areequal ) then
        order = 0
      elseif ( a < b ) then
        order = -1
      else
        order = 1
      end
    end
  end
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

