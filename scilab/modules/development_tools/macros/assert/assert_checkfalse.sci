// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkfalse ( condition )
  //   Check that condition is true.
  //
  // Calling Sequence
  //   flag = assert_checkfalse ( condition )
  //   flag = assert_checkfalse ( condition )
  //   [flag,errmsg] = assert_checkfalse ( condition )
  //
  // Parameters
  //   condition: a matrix of booleans
  //   flag : a 1x1 matrix of boolean, %t if computed is close to expected, %f if not
  //   errmsg : a 1x1 matrix of strings, the error message. If flag==%t, then errormsg=="". If flag==%f, then errmsg contains the error message.
  //
  // Description
  //   Performs silently if all entries in condition are false.
  //
  //   Generates an error if any entry in the condition matrix 
  //   is true.
  //   Generates an error if the condition variable is not a boolean.
  //
  //   If any entry in condition is true, 
  //   <itemizedlist>
  //   <listitem>if the errmsg output variable is not used, an error is generated,</listitem>
  //   <listitem>if the errmsg output variable is used, no error is generated.</listitem>
  //   </itemizedlist>
  //
  // Examples
  //   // Tests which pass
  //   assert_checkfalse ( %f );
  //   flag = assert_checkfalse ( %f )
  //   [flag,errmsg] = assert_checkfalse ( %f )
  //   [flag,errmsg] = assert_checkfalse ( [%f %f] );
  //
  //   // Tests which fail
  //   assert_checkfalse ( [%t %f] );
  //   flag = assert_checkfalse ( [%t %f] )
  //   // No error generated
  //   [flag,errmsg] = assert_checkfalse ( [%t %f] )
  //
  //   // Wrong calls
  //   assert_checkfalse ( "a" )
  //
  // Authors
  //   Michael Baudin, 2010

  [lhs,rhs]=argn()
  if ( rhs <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d are expected.") , "assert_checkfalse" , rhs , 1 )
    error(errmsg)
  end
  //
  // Check types of variables
  if ( typeof(condition) <> "boolean" ) then
    errmsg = sprintf ( gettext ( "%s: Expected boolean variable, but got %s instead.") , "assert_checkfalse" , typeof(condition) )
    error(errmsg)
  end
  //
  if ( and(~condition) ) then
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
      "assert_checkfalse",cstr)
    if ( lhs < 2 ) then
      // If no output variable is given, generate an error
      assert_generror ( errmsg )
    end
  end
endfunction

