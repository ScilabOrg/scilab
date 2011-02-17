// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function assert_generror ( varargin )
    //   Generates an error.
    //
    // Calling Sequence
    //   assert_generror ( errormsg )
    //   assert_generror ( errormsg , errornb )
    //
    // Parameters
    //   expectedmsg : a 1x1 matrix of strings, the error message to be produced
    //   expectederrnb : a 1x1 matrix of floating point integers, the error number (default expectederrnb=[]).
    //
    // Description
    //   Calls the error function, with the given arguments.
    //
    //   This function is called by the assert_check* function each time an error
    //   produced by the wrong match between expected and computed outputs is generated.
    //   In the case where a assert_check* function receives a wrong number of input arguments, 
    //   a wrong number of ouput arguments, a wrong type of input argument or a wrong content 
    //   of input arguments, the regular error function is called.
    //   This function can be customized to modify the behaviour of the assert_check* functions.
    //
    // Examples
    //   // Both these calls generate an error
    //   assert_generror ( "oups" );
    //   assert_generror ( "oups" , 123456789 );
    //
    // // The following call generates an error.
    // assert_checktrue ( [%t %f] );
    // //
    // // Define our own error handler
    // function myerror ( varargin )
    //     [lhs,rhs]=argn()
    //     errormsg = varargin(1)
    //     if ( rhs == 1 ) then
    //         mprintf( "myerror: %s\n", errormsg )
    //     else
    //         errornb = varargin(2)
    //         mprintf( "myerror: %s (%d)\n", errormsg , errornb )
    //     end
    // endfunction
    // //
    // // Configure the new error handler
    // back=funcprot();
    // funcprot(0);
    // olderrorfunction = assert_generror;
    // assert_generror = myerror;
    // funcprot(back);
    // //
    // // Check that the new error handler is in place
    // assert_checktrue ( [%t %f] );
    // //
    // // Put back the regular error handler in place
    // back=funcprot();
    // funcprot(0);
    // assert_generror = olderrorfunction;
    // funcprot(back);
    //
    // Authors
    //   Copyright (C) 2010 - DIGITEO - Michael Baudin

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

