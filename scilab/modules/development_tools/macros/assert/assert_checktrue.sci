// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [flag,errmsg] = assert_checktrue ( condition )
    //   Check that condition is true.

    [lhs,rhs]=argn()
    if ( rhs <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d expected.\n") , "assert_checktrue" , 1 )
        error(errmsg)
    end
    //
    // Check types of variables
    if ( typeof(condition) <> "boolean" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Boolean matrix expected.\n" ) , "assert_checktrue" , 1 )
        error(errmsg)
    end
    //
    if ( and(condition) ) then
        flag = %t
        errmsg = ""
    else
        flag = %f
        if ( size(condition,"*") == 1 ) then
            cstr = string(condition)
        else
            cstr = "[" + string(condition(1)) + " ...]"
        end
        errmsg = msprintf(gettext("%s: Assertion failed: found false entry in condition = %s"), ..
        "assert_checktrue",cstr)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction

