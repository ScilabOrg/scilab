// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

funcprot(0);

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

//
condition = [
 0
 1
 1.e1
 1.e2
 1.e3
 1.e13
 1.e14
 1.e15
 1.e16
 1.e17
 1.e18
];
computed = assert_cond2reqdigits ( condition );
expected = [
    15.954589770191001107946  
    15.954589770191001107946  
    14.954589770191001107946  
    13.954589770191001107946  
    12.954589770191001107946  
    2.954589770191002884303   
    1.9545897701910011079462  
    0.954589770191002884303   
    0.                        
    0.                        
    0.                        
];
MY_assert_equal ( computed , expected );
//
// With offset
computed = assert_cond2reqdigits ( 1.e2 , [5 4 3 2 1 0 -1 -2 -3 -4 -5]' );
expected = [
    15.954589770191001107946  
    15.954589770191001107946  
    15.954589770191001107946  
    15.954589770191001107946  
    14.954589770191001107946  
    13.954589770191001107946  
    12.954589770191001107946  
    11.954589770191001107946  
    10.954589770191001107946  
    9.9545897701910011079462  
    8.9545897701910011079462  
];
MY_assert_equal ( computed , expected );
//
computed = assert_cond2reqdigits ( 1.e14 , [0 -1 -2 -3] );
expected = [1.9545897701910011079462    0.9545897701910011079462    0.    0.];
MY_assert_equal ( computed , expected );
//
computed = assert_cond2reqdigits ( 1.e2 , [0 1 2 3] ) ;
expected = [13.954589770191001107946    14.954589770191001107946    15.954589770191001107946    15.954589770191001107946];
MY_assert_equal ( computed , expected );
//
// Use base-2
condition = [
 0
 1
 1.e1
 1.e2
 1.e3
 1.e13
 1.e14
 1.e15
 1.e16
 1.e17
 1.e18
];
computed = assert_cond2reqdigits ( condition , [] , 2 );
expected = [
    53.                       
    53.                       
    49.678071905112638262381  
    46.3561438102252765248    
    43.034215715337914787142  
    9.814934766464290305521   
    6.4930066715769214624743  
    3.1710785766895668302823  
    0.                        
    0.                        
    0.                        
];
MY_assert_equal ( computed , expected );
//
// With offset
computed = assert_cond2reqdigits ( 1.e2 , [5 4 3 2 1 0 -1 -2 -3 -4 -5]' , 2 );
expected = [
    51.3561438102252765248  
    50.3561438102252765248  
    49.3561438102252765248  
    48.3561438102252765248  
    47.3561438102252765248  
    46.3561438102252765248  
    45.3561438102252765248  
    44.3561438102252765248  
    43.3561438102252765248  
    42.3561438102252765248  
    41.3561438102252765248  
];
MY_assert_equal ( computed , expected );

funcprot(1);
