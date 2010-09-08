
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function fr=freson(h)

[lhs,rhs]=argn(0)
[n,d]=h(['num','den']);
if type(n)==1 then n=poly(n,varn(d),'c'),end
d0=coeff(d,0)
if d0==0 then
   error('infinite gain at zero frequency'),
end;
ar0=abs(coeff(n,0)/d0)^2
//look for  omega such that derivative of magn. is zero
niw=horner(n,%i*poly(0,'w'));
diw=horner(d,%i*poly(0,'w'))
niw=real(niw*conj(niw));diw=real(diw*conj(diw));
modul_d=derivat(niw/diw);w=roots(modul_d(2));

// get extreme points
k=find(imag(w)==0&real(w)>=0)
if k==[] then fr=[],g=[],return,end
w=real(w(k))

//find maximums
wx=max(w)+0.5
if horner(modul_d,wx)<0 then
  w=w($:-2:1)
else
  w=w($-1:-2:1)
end
fr=w/(2*%pi)
endfunction
