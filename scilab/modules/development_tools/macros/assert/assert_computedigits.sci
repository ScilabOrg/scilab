// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d = assert_computedigits ( varargin )
  // Returns the number of significant digits in computed result.
  //
  // Calling Sequence
  //   d = assert_computedigits ( computed , expected )
  //   d = assert_computedigits ( computed , expected , basis )
  //
  // Parameters
  //   computed : a matrix of doubles, the computed value
  //   expected : a matrix of doubles, the expected value
  //   basis : a 1x1 matrix of floating point integers, the basis (default basis=10)
  //   d : a matrix of doubles, the number of significant digits.
  //
  // Description
  //   Computes the number of significant digits in a computed 
  //   result with respect to a nonzero expected result, using the formula:
  //
  //<latex>
  //\begin{eqnarray}
  //d = - \frac{\log_{10} ( r )}{\log_{10}(basis)}
  //\end{eqnarray}
  //</latex>
  //
  // where r is the relative error defined by 
  //
  //<latex>
  //\begin{eqnarray}
  //r = \frac{|computed - expected|}{|expected|}
  //\end{eqnarray}
  //</latex>
  // 
  //   Any optional input argument equal to the empty matrix is replaced by its default value.
  //
  // The number of significant digits is between dmin = 0 and dmax = -log10(2^(-53)) which 
  // is approximately 15.95 ~ 16.
  // In base 2, the number of significant bits is 53.
  //
  // If expected is equal to computed, then d is set to its maximum value, i.e. dmax.
  // If not, if expected is zero and computed is nonzero, then d is set to its minimum 
  // value, i.e. dmin=0.
  //
  // The values of computed and expected cannot be exchanged, since the relative 
  // error is based on the expected value.
  //
  // The computation separates the real part and the imaginary parts of the 
  // values.
  // The returned number of digits is the minimum of the number of digits for the 
  // real and imaginary parts.
  //
  // TODO : use computedigits inside a assert_digits ( computed , expected , dmin ) function
  //
  // TODO : add a comptype option to make the comparison "matrix" (current is "element")
  //
  // Examples
  //   d = assert_computedigits ( 1 , 1 ) // d ~ 16
  //   d = assert_computedigits ( 1 , 1 , 2 ) // d = 53
  //   d = assert_computedigits ( 0 , 0 ) // d ~ 16
  //   d = assert_computedigits ( 1 , 0 ) // d = 0
  //   d = assert_computedigits ( 0 , 1 ) // d = 0
  //   d = assert_computedigits ( 3.1415926 , %pi ) // d ~ 8
  //   d = assert_computedigits ( 3.1415926 , %pi , 2 ) // d ~ 26
  //   d = assert_computedigits ( [0 0 1 1] , [0 1 0 1] ) // d ~ [16 0 0 16]
  //   d = assert_computedigits(ones(3,2),ones(3,2)) // d ~ 16 * ones(3,2)
  //   d = assert_computedigits(1.224646799D-16,8.462643383D-18) // d = 0
  //
  //   // Check IEEE values
  //   // d ~ [16 0 0 0]
  //   d = assert_computedigits([%nan %nan %nan %nan],[%nan %inf -%inf 0]) 
  //   // d ~ [0 16 0 0]
  //   d = assert_computedigits([%inf %inf %inf %inf],[%nan %inf -%inf 0]) 
  //   // d = [0 0 16 0]
  //   d = assert_computedigits([-%inf -%inf -%inf -%inf],[%nan %inf -%inf 0]) 
  //   // d = [0 0 0 16]
  //   d = assert_computedigits([0 0 0 0],[%nan %inf -%inf 0]) 
  //
  //   // Check complex values
  //   d = assert_computedigits ( 1.2345 + %i*6.7891 , 1.23456789 + %i*6.789123456 ) // d ~ 4
  //
  // Authors
  //   Michael Baudin, Digiteo, 2009-2010
  //

  [lhs,rhs]=argn()
  if ( and ( rhs <> [2 3] ) ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while from %d to %d are expected.") , "assert_computedigits" , rhs , 2 , 3 )
    error(errmsg)
  end
  computed = varargin ( 1 )
  expected = varargin ( 2 )
  basis = argindefault ( rhs , varargin , 3 , 10 )
  //
  // Check types of variables
  if ( typeof(computed) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_computedigits" , 1 , "computed" , typeof(computed) , "constant" )
    error(errmsg)
  end
  if ( typeof(expected) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_computedigits" , 2 , "expected" , typeof(expected) , "constant" )
    error(errmsg)
  end
  //
  // Check sizes of variables
  if ( size(expected) <> size(computed) ) then
    errmsg = sprintf ( gettext ( "%s: The size of computed is [%d,%d] while the size of expected is [%d,%d].") , "assert_computedigits" , size(expected,"r") , size(expected,"c") , size(computed,"r") , size(computed,"c") )
    error(errmsg)
  end
  if ( size(basis,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_condnum" , 3 , "basis" , size(basis,"*") , 1 )
    error(errmsg)
  end
  //
  nre = size(expected,"r")
  nce = size(expected,"c")
  // Update shape
  expected = expected (:)
  computed = computed (:)
  //
  d = zeros(expected)
  //
  n = size(expected,"*")
  for i = 1 : n
    dre = computedigits_data ( real(expected(i)) , real(computed(i)) , basis )
    dim = computedigits_data ( imag(expected(i)) , imag(computed(i)) , basis )
    d(i) = min(dre,dim)
  end
  //
  // Reshape
  d = matrix(d,nre,nce)
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
function d = computedigits_data ( expected , computed , basis )
    dmin = 0
    dmax = -log(2^(-53))/log(basis)
    //
    if ( isnan(expected) & isnan(computed) ) then
        d = dmax
    elseif ( isnan(expected) & ~isnan(computed) ) then
        d = dmin
    elseif ( ~isnan(expected) & isnan(computed) ) then
        d = dmin
        // From now, both expected and computed are non-nan
    elseif ( expected == 0 & computed == 0 ) then
        d = dmax
    elseif ( expected == 0 & computed <> 0 ) then
        d = dmin
        // From now, expected is non-zero
    elseif ( expected == computed ) then
        d = dmax
        // From now, expected and computed are different
    elseif ( expected == %inf & computed <> %inf ) then
        d = dmin
    elseif ( expected == -%inf & computed <> -%inf ) then
        d = dmin
        // From now, neither of computed, nor expected is infinity
    else
        // The max function does not ensure that the sign bit of d is positive.
        // For example : 
        // ieee(2); z=max(-0,0); 1/z is -%inf
        // To force this, consider the special case where the relative error is 
        // larger than 1.
        relerr = abs(computed-expected) / abs(expected)
        if ( relerr >= 1 ) then
            d = dmin
        else
            sigdig = -log ( relerr ) / log(basis)
            d = max ( sigdig , dmin )
        end
    end
endfunction

