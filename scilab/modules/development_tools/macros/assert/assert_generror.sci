// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function assert_generror ( varargin )
    //   Generates an error.

    [lhs,rhs]=argn()
    if ( and(rhs <> [1 2] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_generror" , 1 , 2 )
        error(errmsg)
    end
    errormsg = varargin(1)
    //
    // Check types of variables
    if ( typeof(errormsg) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: A character string expected.") , "assert_generror" , 1 )
        error(errmsg)
    end
    if ( size(errormsg,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_generror" , 1 , "errormsg" , size(errormsg,"*") , 1 )
        error(errmsg)
    end
    //
    if ( rhs == 1 ) then
        error ( errormsg )
    else
        errornb = varargin(2)
        if ( size(errornb,"*") <> 1 ) then
            errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_generror" , 2 , "errornb" , size(errornb,"*") , 1 )
            error(errmsg)
        end
        if ( typeof(errornb) <> "constant" ) then
            errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Real matrix expected.") , "assert_generror" , 2 )
            error(errmsg)
        end
        if ( errornb < 0 ) then
            errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_generror" , 2 , "errornb" )
            error(errmsg)
        end
        error ( errormsg , errornb )
    end
endfunction

