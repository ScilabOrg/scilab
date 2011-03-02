// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkequal ( computed , expected )
    //  Check that computed and expected are equal.

    function flag = comparedoubles ( computed , expected )
        [cnonan , cnumb] = mythrownan(computed)
        [enonan , enumb] = mythrownan(expected)
        if ( and(enonan == cnonan) & and(enumb == cnumb) ) then
            flag = %t
        else
            flag = %f
        end
    endfunction

    function [nonan,numb] = mythrownan(x)
        //
        //
        // Copyright (C) 2000 - INRIA - Carlos Klimann
        // Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
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
        if ( rhs<>1 ) then 
            error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"thrownan",1))
        end
        numb=find(bool2s(~isnan(x)))
        nonan=x(~isnan(x))
    endfunction

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
        cisreal = isreal(computed)
        eisreal = isreal(expected)
        if ( cisreal & ~eisreal ) then
            errmsg = sprintf ( gettext ( "%s: Computed is real, but expected is complex.") , "assert_checkequal" )
            error(errmsg)
        end
        if ( ~cisreal & eisreal ) then
            errmsg = sprintf ( gettext ( "%s: Computed is complex, but expected is real.") , "assert_checkequal" )
            error(errmsg)
        end
        if ( cisreal & eisreal ) then
            flag = comparedoubles ( computed , expected )
        else
            flagreal = comparedoubles ( real(computed) , real(expected) )
            if ( flagreal ) then
                flagimag = comparedoubles ( imag(computed) , imag(expected) )
                flag = flagimag
            else
                flag = %f
            end
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


