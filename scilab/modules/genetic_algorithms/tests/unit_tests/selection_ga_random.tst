
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

deff('y=test_func(x)','y=x^2');

Pop_in = list();
Indiv1 = list();
Indiv2 = list();
FObj_Pop_in = [];
FObj_Indiv1 = [];
FObj_Indiv2 = [];

for i=1:100
  Pop_in(i) = rand();
  Indiv1(i) = rand();
  Indiv2(i) = rand();
  FObj_Pop_in(i) = test_func(Pop_in(i));
  FObj_Indiv1(i) = test_func(Indiv1(i));
  FObj_Indiv2(i) = test_func(Indiv2(i));
end

[Pop_out,FObj_Pop_out,Efficiency,MO_Total_FObj_out] = selection_ga_random(Pop_in,Indiv1,Indiv2,FObj_Pop_in,FObj_Indiv1,FObj_Indiv2, [],[],[],[]);

if length(Pop_out)~=100 then pause,end
if size(Pop_out)~=size(Pop_in) then pause,end
if length(Efficiency)~=100 then pause,end
if ~isempty(MO_Total_FObj_out) then pause,end
