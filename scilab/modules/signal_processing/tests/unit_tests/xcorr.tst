// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

x=[1 2 3 4 3 2 1];
c_ref=[1 4 10 20 31 40 44 40 31 20 10 4 1];
assert_checkalmostequal(c_ref,xcorr(x));
assert_checkalmostequal(c_ref,xcorr(x,"none"));
assert_checkalmostequal(c_ref/7,xcorr(x,"biased"));
assert_checkalmostequal(c_ref/44,xcorr(x,"coeff"));
[c,ind]=xcorr(x);
assert_checkalmostequal(c_ref./(7-abs(ind)),xcorr(x,"unbiased"));

assert_checkalmostequal(c_ref(4:$-3),xcorr(x,3));
assert_checkalmostequal(c_ref(4:$-3),xcorr(x,3,"none"));
assert_checkalmostequal(c_ref(4:$-3)/7,xcorr(x,3,"biased"));
assert_checkalmostequal(c_ref(4:$-3)/44,xcorr(x,3,"coeff"));
[c,ind]=xcorr(x,3,"unbiased");
assert_checkalmostequal(c_ref(4:$-3)./(7-abs(ind)),c);

x=[1 2*%i 3 4 3 2 1];
c_ref=[ 1,2+%i*2,6+%i*4,14+%i*6,23+%i*8,32+%i*4,44,...
        32-%i*4,23-%i*8,14-%i*6,6-%i*4,2-%i*2,1];
assert_checkalmostequal(c_ref,xcorr(x));
assert_checkalmostequal(c_ref(4:$-3),xcorr(x,3));

x=[1 2 3 4 3 2 1];
y=[4 3 2 1];
c_ref=[0,0,0, 1, 4, 10, 20, 28, 32, 30, 20, 11, 4];
c=xcorr(x,y);
assert_checktrue(isreal(c));
assert_checkalmostequal(c_ref,c,sqrt(%eps),20*%eps);
assert_checkalmostequal(c_ref,xcorr(x,y,"none"),sqrt(%eps),20*%eps);
assert_checkalmostequal(c_ref/7,xcorr(x,y,"biased"),sqrt(%eps),20*%eps);
assert_checkalmostequal(c_ref/(norm(x)*norm(y)),xcorr(x,y,"coeff"),sqrt(%eps),20*%eps);


assert_checkalmostequal(c_ref(4:$-3),xcorr(x,y,3));
assert_checkalmostequal(c_ref(4:$-3),xcorr(x,y,3,"none"));
assert_checkalmostequal(c_ref(4:$-3)/7,xcorr(x,y,3,"biased"));
assert_checkalmostequal(c_ref(4:$-3)/(norm(x)*norm(y)),xcorr(x,y,3,"coeff"));
[c,ind]=xcorr(x,y,3,"unbiased");
assert_checkalmostequal(c_ref(4:$-3)./(7-abs(ind)),c);

x=[1 2 3 4 3 2 1];
y=[4 3 2 1+%i];
c_ref=[0,0,0, 1-%i, 4-%i*2, 10-%i*3, 20-%i*4, 28-%i*3, 32-%i*2, 30-%i, 20, 11, 4];

c=xcorr(x,y);
assert_checkalmostequal(c_ref,c,sqrt(%eps),20*%eps);
assert_checkalmostequal(c_ref(4:$-3),xcorr(x,y,3));
