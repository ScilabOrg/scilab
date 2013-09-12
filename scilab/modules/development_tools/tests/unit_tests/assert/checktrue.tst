// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function checkassert ( flag , errmsg , ctype )
  if ( ctype == "success" ) then
    MY_assert_equal ( (flag==%t) & (errmsg==""), %t )
  else
    MY_assert_equal ( (flag==%f) & (errmsg<>""), %t )
  end
endfunction

format("v",10);

// Check error message when number of arguments is false
instr = "assert_checktrue ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "[o1,o2,o3]=assert_checktrue ( %t )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 59 );
//////////////////////////////////////////
// Check error message when type of arguments is false
instr = "assert_checktrue ( ""a"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
//
// Check that the error message is correctly handled.
instr = "assert_checktrue ( [%f %t] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
errmsg = lasterror();
refmsg = msprintf( gettext( "%s: Assertion failed: found false entry in condition = %s" ) , "assert_checktrue", "[F ...]");
MY_assert_equal ( errmsg , refmsg );
//
[flag,errmsg] = assert_checktrue ( %t );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checktrue ( [%t %t] );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checktrue ( %f );
checkassert ( flag , errmsg , "failure" );
//
[flag,errmsg] = assert_checktrue ( [%t %f] );
checkassert ( flag , errmsg , "failure" );

