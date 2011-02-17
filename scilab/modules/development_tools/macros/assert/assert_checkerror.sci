// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkerror ( varargin )
    //  Check that an instruction produces the expected error.
    //
    // Calling Sequence
    //   flag = assert_checkerror ( instr , expectedmsg )
    //   flag = assert_checkerror ( instr , expectedmsg , expectederrnb )
    //   [flag,errmsg] = assert_checkerror ( ... )
    //
    // Parameters
    //   instr: a 1x1 matrix of strings, an expression to evaluate
    //   expectedmsg : a 1x1 matrix of strings, the error message to be produced
    //   expectederrnb : a 1x1 matrix of floating point integers, the error number (default expectederrnb=[]).
    //   flag : a 1x1 matrix of boolean, %t if computed is close to expected, %f if not
    //   errmsg : a 1x1 matrix of strings, the error message. If flag==%t, then errormsg=="". If flag==%f, then errmsg contains the error message.
    //
    // Description
    //   If the expression does not generate an error, then assert_checkerror generates an error. 
    //   Performs silently if the evaluated expression generates the expected error message. 
    //   If the error number is provided, we additionnally check that the generated error number matches the 
    //   expected one. 
    //
    //   The string matching is strict string equality: no pattern or regular expression can be used.
    //
    //   If the error message is not expected or the error number is not expected,
    //   <itemizedlist>
    //   <listitem>if the errmsg output variable is not used, an error is generated,</listitem>
    //   <listitem>if the errmsg output variable is used, no error is generated.</listitem>
    //   </itemizedlist>
    //
    //   The goal of this function is to enable the tester to check the error 
    //   cases in a simplified framework.
    //
    // Examples
    // // This function generates an error when
    // // * the number of input arguments in wrong
    // // * the type of x is wrong
    // function y = f(x)
    //     [lhs,rhs]=argn()
    //     if ( rhs <> 1 ) then
    //         errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "f" , 1 , 1 )
    //         error(errmsg)
    //     end
    //     if ( typeof(x) <> "constant" ) then
    //         localstr = gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.")
    //         errmsg = sprintf ( localstr , "f" , 1 , "x" , typeof(x) , "constant" )
    //         error(errmsg,123456789)
    //     end
    //     y = x
    // endfunction
    // // Our task is to check the error messages produced by this function
    // // Test which pass : the error message is the expected one
    // msg1="f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected.";
    // msg2="f: Unexpected type of input argument #1 : variable x has type string while constant is expected.";
    // assert_checkerror ( "y=f()"      , msg1 );
    // assert_checkerror ( "y=f(""a"")" , msg2 );
    // // Also check the error number
    // assert_checkerror ( "y=f()"      , msg1 , 10000 );
    // assert_checkerror ( "y=f(""a"")" , msg2 , 123456789 );
    // // Test which fail
    // assert_checkerror ( "y=f()"      , "oups" );
    // assert_checkerror ( "y=f()"      , msg1 , 12 );
    //
    // // When errmsg is given as output argument, no error is generated
    // // A test which pass: flag is %t, errmsg is empty
    // [flag,errmsg] = assert_checkerror ( "y=f()"      , msg1 )
    // // A test which fail: flag is %f, errmsg is not empty
    // [flag,errmsg] = assert_checkerror ( "y=f()"      , "oups" )
    //
    // Authors
    //   Copyright (C) 2010 - DIGITEO - Michael Baudin

    [lhs,rhs]=argn()
    if ( and(rhs <> [2 3] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_checkerror" , 2 , 3 )
        error(errmsg)
    end
    //
    // Get arguments
    instr = varargin(1)
    expectedmsg = varargin(2)
    expectederrnb = argindefault ( rhs , varargin , 3 , [] )
    //
    // Check types of variables
    if ( typeof(instr) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: A character string expected.") , "assert_checkerror" , 1 )
        error(errmsg)
    end
    if ( typeof(expectedmsg) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: A character string expected.") , "assert_checkerror" , 2 )
        error(errmsg)
    end
    if ( typeof(expectederrnb) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Real matrix expected.") , "assert_checkerror" , 3 )
        error(errmsg)
    end  
    //
    // Check sizes of variables
    if ( size(instr,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkerror" , 1 , "instr" , size(instr,"*") , 1 )
        error(errmsg)
    end
    if ( size(expectedmsg,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkerror" , 2 , "expectedmsg" , size(expectedmsg,"*") , 1 )
        error(errmsg)
    end
    if ( expectederrnb <> [] ) then
        if ( size(expectederrnb,"*") <> 1 ) then
            errmsg = sprintf ( gettext ( "%s: Unexpected size of input argument #%d : variable %s has size %d while %d is expected.") , "assert_checkerror" , 3 , "expectederrnb" , size(expectederrnb,"*") , 1 )
            error(errmsg)
        end
    end
    //
    // Check values of variables
    if ( expectederrnb <> [] ) then
        if ( expectederrnb < 0 ) then
            errmsg = sprintf ( gettext ( "%s: Unexpected value input argument #%d : variable %s has negative entries.") , "assert_checkerror" , 3 , "expectederrnb" )
            error(errmsg)
        end
    end
    //
    // Proceed...
    compierr = execstr(instr,"errcatch")
    //
    // Check that an error is produced
    if ( compierr == 0 ) then
        localstr = gettext ( "%s: No error was produced while evaluating ""%s"".")
        errmsg = sprintf ( localstr , "assert_checkerror" , instr )
        error(errmsg)
    end
    //
    // Get the error
    compmsg = lasterror()
    //
    // Initialize output arguments
    flag = %t
    errmsg = ""
    //
    // Check the error message
    if ( expectedmsg <> compmsg ) then
        flag = %f
        localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".")
        errmsg = msprintf(localstr,"assert_checkerror",expectedmsg,compmsg)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            error ( errmsg )
        else
            return
        end
    end
    if ( expectederrnb <> [] ) then
        //
        // Check the error number
        if ( expectederrnb <> compierr ) then
            flag = %f
            localstr = gettext("%s: Assertion failed: expected error number = %d while computed error number = %d.")
            errmsg = msprintf(localstr,"assert_checkerror",expectederrnb,compierr)
            if ( lhs < 2 ) then
                // If no output variable is given, generate an error
                error ( errmsg )
            else
                return
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

