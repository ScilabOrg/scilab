// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkequal ( computed , expected )
    //  Check that computed and expected are equal.
    //
    // Calling Sequence
    //   assert_checkequal ( computed , expected )
    //   flag = assert_checkequal ( computed , expected )
    //   [flag,errmsg] = assert_checkequal ( computed , expected )
    //
    // Parameters
    //   computed: the computed result
    //   expected : the expected result
    //   flag : a 1x1 matrix of boolean, %t if computed is close to expected, %f if not
    //   errmsg : a 1x1 matrix of strings, the error message. If flag==%t, then errormsg=="". If flag==%f, then errmsg contains the error message.
    //
    // Description
    //   Performs silently if computed and expected are equal.
    //
    //   If the type of both input arguments is 1 (i.e. a real matrix),
    //   we check that non-nan values are equal.
    //
    //   If the comparison shows that computed is equal to expected, 
    //   and if the errmsg output variable is not used, an error is generated.
    //   If the comparison shows that computed is equal to expected, 
    //   and if the errmsg output variable is used, no error is generated.
    //
    // Examples
    // // Tests with success
    // assert_checkequal ( %T , %T );
    // flag = assert_checkequal ( list() , list() );
    // [flag , errmsg] = assert_checkequal ( [%T %F], [%T %F] );
    // assert_checkequal ( %nan , %nan );
    // 
    // // Tests with failure
    // assert_checkequal ( %F , %T );
    // flag = assert_checkequal ( %F , %T );
    // // No error produced :
    // [flag , errmsg] = assert_checkequal ( %F , %T )
    // assert_checkequal ( [1 2], [3 4] )
    // assert_checkequal ( 1 , [2 3] )
    // assert_checkequal ( 1 , "b" )
    //
    // Authors
    //   Michael Baudin, 2010
    //
    // Bibliography
    //   "Automated Software Testing for Matlab", Steven Eddins, 2009

    [lhs,rhs]=argn()
    if ( rhs <> 2 ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d are expected.") , "assert_checkequal" , rhs , 2 )
        error(errmsg)
    end
    //
    // Check types of variables
    if ( typeof(computed) <> typeof(expected) ) then
        errmsg = sprintf ( gettext ( "%s: Variable typeof(computed) = %s while typeof(expected) = %s: inconsistent match.") , "assert_checkequal" , typeof(computed) , typeof(expected) )
        error(errmsg)
    end
    //
    // Check sizes of variables
    ncom = size(computed)
    nexp = size(expected)
    if ( or(ncom <> nexp) ) then
        cstr = "[" + strcat(string(ncom)," ") + "]"
        estr = "[" + strcat(string(nexp)," ") + "]"
        errmsg = sprintf ( gettext ( "%s: We have size(computed) = %s while size(expected) = %s: not consistent comparison.") , "assert_checkequal" , cstr , estr )
        error(errmsg)
    end  
    //
    if ( type(computed) == 1 & type(expected) == 1 ) then
        // These are two matrices of doubles
        csize = size (computed,"*")
        esize = size (expected,"*")
        [cnonan , cnumb] = mythrownan(computed)
        [enonan , enumb] = mythrownan(expected)
        if ( and(enonan == cnonan) & and(enumb == cnumb) ) then
            flag = %t
        else
            flag = %f
        end
    else
        if ( and ( computed == expected ) ) then
            flag = %t
        else
            flag = %f
        end
    end
    if ( flag == %t ) then
        errmsg = ""
    else
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
        errmsg = msprintf(gettext(..
        "%s: Assertion failed: expected = %s while computed = %s"), ..
        "assert_checkequal",estr,cstr)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction

function [nonan,numb] = mythrownan(x)
    //
    //
    // Copyright (C) 2000 - INRIA - Carlos Klimann
    // Copyright (C) 2010 - DIGITEO - Michael Baudin
    //This  function  returns  in  vector  nonan  the  values
    //(ignoring the NANs) of a  vector or matrix x and in the
    //corresponding places of vector  numb the indexes of the
    //value.
    //
    //For  a  vector  or matrix  x,  [nonan,numb]=thrownan(x)
    //considers x, whatever his dimensions are, like a linear
    //vector (columns  first).
    //
    //
    [lhs,rhs]=argn(0)
    if rhs<>1 then error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"thrownan",1)), end
    numb=find(bool2s(~isnan(x)))
    nonan=x(~isnan(x))
endfunction

