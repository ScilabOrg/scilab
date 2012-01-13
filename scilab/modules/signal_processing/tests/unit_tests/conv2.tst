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
assert_checkequal(conv2(s,A),reffull);
assert_checkequal(conv2(A,s),reffull);

assert_checkequal(conv2(s,A,'full'),reffull);
assert_checkequal(conv2(A,s,'full'),reffull);

assert_checkequal(conv2(s,A,'same'),[0,0,0;0,0,0;-4,-4,-3]);
refsame=zeros(10,10);
refsame([2,3,7,8],2:8)=[1;1;-1;-1]*[1 3 4 4 4 3 1];
assert_checkequal(conv2(A,s,'same'),refsame);

assert_checkequal(conv2(s,A,'valid'),[]);
refvalid=zeros(8,8);
refvalid([1,2,6,7],1:7)=[1;1;-1;-1]*[1 3 4 4 4 3 1];
assert_checkequal(conv2(A,s,'valid'),refvalid);

u=[1;0;-1];v=[1 2 1];
assert_checkequal(conv2(u,v,A),reffull);
assert_checkequal(conv2(u,v,A,'full'),reffull);
assert_checkequal(conv2(u,v,A,'same'),refsame);
assert_checkequal(conv2(u,v,A,'valid'),refvalid);

a=[1 2 3;4 5 6;7 8 9];

assert_checkequal(conv2(a,s,'same'),[13 20 17;18 24 18;-13 -20 -17]);

assert_checkequal(conv2([3;4],[1;2]),[3;10;8]);

assert_checkequal(conv2([1;2],1,[3;4]),[3;10;8]);
assert_checkequal(conv2([1;2],%i,[3;4]),[3;10;8]*%i); 
assert_checkequal(conv2([1;2]*%i,1,[3;4]),[3;10;8]*%i);
assert_checkequal(conv2([1;2],1,[3;4]*%i),[3;10;8]*%i);
assert_checkequal(conv2([1;2],%i,[3;4]*%i),-[3;10;8]+0*%i);
assert_checkequal(conv2([1;2]*%i,%i,[3;4]),-[3;10;8]+0*%i);
assert_checkequal(conv2([1;2]*%i,%i,[3;4]*%i),-[3;10;8]*%i);

assert_checkequal(conv2([1 2;3 4],[]),zeros(2,2));
assert_checkequal(conv2([],[1 2;3 4]),zeros(2,2));
assert_checkequal(conv2([1 2;3 4],[],'same'),zeros(2,2));
assert_checkequal(conv2([],[1 2;3 4],'same'),[]);
assert_checkequal(conv2([1 2;3 4],[],'valid'),zeros(2,2));
assert_checkequal(conv2([],[1 2;3 4],'valid'),[]);

//here we do not follow matlab
assert_checkequal(conv2([],[1,2],[1 2;3 4]),zeros(2,2));
assert_checkequal(conv2([1,2],[],[1 2;3 4]),zeros(2,2));
assert_checkequal(conv2([],[1;2],[1 2;3 4]),zeros(2,2));
assert_checkequal(conv2([1;2],[],[1 2;3 4]),zeros(2,2));
