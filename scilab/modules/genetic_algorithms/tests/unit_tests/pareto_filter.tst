
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

function Res = min_bd_deb_1(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_1(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_1(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_1(x)
f1_x1 = x(1);
g_x2  = 1 + 9 * sum((x(2:$)-x(1)).^2) / (length(x) - 1);
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

Max = max_bd_deb_1(2);
Min = min_bd_deb_1(2);

X_in = list();
for i=1:100
  X_in(i) = (Max - Min) .* rand(size(Max,1),size(Max,2)) + Min;
  F_in(i,:) = deb_1(X_in(i));
end

[F_out, X_out, Ind_out] = pareto_filter(F_in, X_in);

if length(X_out)==0 then pause,end
if size(X_out(1))~=size(X_in(1)) then pause,end
if length(X_out)~=length(Ind_out) then pause,end
