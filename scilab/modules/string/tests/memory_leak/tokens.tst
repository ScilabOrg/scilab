// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//======================================
// test tokens
// memory leak
//======================================
clear;
FUNCTION_TO_TEST = "tokens";
NB_LOOPS = 1000000;
REF_4_1_2 = 180;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();

for i=1:NB_LOOPS
    r = tokens("y=a+b*2",["=","+","*"]);
    clear r;
end

[AFTER_FREE,AFTER_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (abs(BEFORE_USED - AFTER_USED) > REF_4_1_2 ) then
    warning(FUNCTION_TO_TEST+" MEMORY LEAK : "+string(abs(BEFORE_USED - AFTER_USED))+ " ko" );
end
//====================================
