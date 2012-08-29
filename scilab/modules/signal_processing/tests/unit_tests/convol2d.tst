// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
s = [1 2 1; 0 0 0; -1 -2 -1];
A = zeros(10,10);
A(3:7,3:7) = 1;
reffull=zeros(12,12);
reffull([3,4,8,9],3:9)=[1;1;-1;-1]*[1 3 4 4 4 3 1];
assert_checkalmostequal(convol2d(s,A),reffull,100*%eps,100*%eps);
assert_checkalmostequal(convol2d(A,s),reffull,100*%eps,100*%eps);


assert_checkequal(convol2d([3;4],[1;2]),[3;10;8]);
assert_checkequal(convol2d([3;4],[1;2]*%i),[3;10;8]*%i);
assert_checkequal(convol2d([3;4]*%i,[1;2]*%i),-[3;10;8]);


assert_checkequal(convol2d([1 2;3 4],[]),zeros(2,2));
assert_checkequal(convol2d([],[1 2;3 4]),zeros(2,2));
