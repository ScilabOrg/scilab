// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

beers=["Desperados", "Leffe", "Kronenbourg", "Heineken"];
if find(beers=="Leffe") <> 2 then pause, end
if find(beers=="1664") <> [] then pause, end
if find(beers=="Foster") <> [] then pause, end
beers=[beers, "Foster"];
if find(beers=="Foster") <> 5 then pause, end

A=[0.3873779    0.9222899    0.9488184    0.3435337    0.3760119];
if find(A<0.4) <> [1 4 5] then pause, end
if find(A>0.4) <> [2 3] then pause, end

A=hypermat([2,3,2],1:12);
[i,j,k]=find(A>5);
if A(i(1),j(1),k(1)) <> 6 then pause, end

[i,j,k]=find(A<=5);
if A(i(1),j(1),k(1)) <> 1 then pause, end

[i,j,k]=find(A>=5);
if A(i(1),j(1),k(1)) <> 5 then pause, end


[i,j,k]=find(A==12);
if A(i(1),j(1),k(1)) <> 12 then pause, end

