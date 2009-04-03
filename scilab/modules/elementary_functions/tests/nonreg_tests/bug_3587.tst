// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// 

// <-- Non-regression test for bug 3587 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3587
//
// <-- Short Description -->
// gsort is not stable: It does not preserve the relative order of equal values

a=[3,3,2,0,4,0,2,2,5,3];

[s,k]=gsort(a,'g','i');
if or(diff(s)<0) then pause,end
if or(k<>[4,6,3,7,8,1,2,10,5,9]) then pause,end

[s,k]=gsort(a,'g','d');
if or(diff(s)>0) then pause,end
if or(k<>[9,5,1,2,10,3,7,8,4,6]) then pause,end

a=int32(a);
[s,k]=gsort(a,'g','i');
if or(diff(s)<0) then pause,end
if or(k<>[4,6,3,7,8,1,2,10,5,9]) then pause,end

[s,k]=gsort(a,'g','d');
if or(diff(s)>0) then pause,end
if or(k<>[9,5,1,2,10,3,7,8,4,6]) then pause,end

a=uint32(a);
[s,k]=gsort(a,'g','i');
if or(diff(int32(s))<0) then pause,end
if or(k<>[4,6,3,7,8,1,2,10,5,9]) then pause,end

[s,k]=gsort(a,'g','d');
if or(diff(int32(s))>0) then pause,end
if or(k<>[9,5,1,2,10,3,7,8,4,6]) then pause,end

a=uint16(a);
[s,k]=gsort(a,'g','i');
if or(diff(int32(s))<0) then pause,end
if or(k<>[4,6,3,7,8,1,2,10,5,9]) then pause,end

[s,k]=gsort(a,'g','d');
if or(diff(int32(s))>0) then pause,end
if or(k<>[9,5,1,2,10,3,7,8,4,6]) then pause,end

a=string([3,3,2,0,4,0,2,2,5,3]);
[s,k]=gsort(a,'g','i');
if or(s<>["0","0","2","2","2","3","3","3","4","5"]) then pause,end
if or(k<>[4,6,3,7,8,1,2,10,5,9]) then pause,end

[s,k]=gsort(a,'g','d');
if or(s<>["5","4","3","3","3","2","2","2","0","0"]) then pause,end
if or(k<>[9,5,1,2,10,3,7,8,4,6]) then pause,end


a=[2,6,6,2;
   8,8,7,3;
   0,7,7,2;
   3,9,2,9;
   8,1,5,2];

[s,k]=gsort(a,'r','i');
if or(s<>[0,1,2,2;2,6,5,2;3,7,6,2;8,8,7,3;8,9,7,9]) then pause,end
for i=1:4,if or (a(k(:,i),i)<>s(:,i)) then pause,end,end
if or(k<>[3,5,4,1;1,1,5,3;4,3,1,5;2,2,2,2;5,4,3,4]) then pause,end

[s,k]=gsort(a,'c','d');
if or(s<>[6,6,2,2;8,8,7,3;7,7,2,0;9,9,3,2;8,5,2,1]) then pause,end
if or(k<>[2,3,1,4;1,2,3,4;2,3,4,1;2,4,1,3;1,3,4,2]) then pause,end
