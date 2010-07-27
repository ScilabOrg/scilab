// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid Belahcene
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=zgrid()
	
	fig = gcf();
	show_window();
	immediate_drawing=fig.immediate_drawing;
	fig.immediate_drawing = "off";
	
	square(-1.1,-1.1,1.1,1.1);
	xtitle( ["loci with constant damping and constant frequencies";"in discrete plane"]," "," ");
	
	xsi=0:0.1:1
	
	//                           2                 2
	//roots of                  s  + 2*xsi*w0*s +w0
	//given by : w0*(-xsi+-%i*sqrt(1-sxi*xsi))
	
	raci=((0:0.05:1)*%pi)'*(-xsi+%i*sqrt(ones(xsi)-xsi.*xsi))
	// continuous --> discrete
	raci=exp(raci);[mr,nr]=size(raci);
	for l=1:nr,
		xstring(real(raci(mr-10,l)),-imag(raci(mr-10,l)),' '+string(xsi(l)),0,0);
	end;
	
	plot2d(real(raci),imag(raci),1*ones(1,nr),"001");
	plot2d(real(raci),-imag(raci),1*ones(1,nr),"001");
	
	w0=(0:0.1:1)*%pi;
	e_itheta=exp(%i*(%pi/2:0.05:%pi)')
	zw=exp(e_itheta*w0);[mz,nz]=size(zw)
	
	for l=1:nz,
		xstring(real(zw(1,l)),imag(zw(1,l)),' '+string(w0(l)/(2*%pi)),0,0);
	end;
	
	//-- the curves
	plot2d(real(zw),imag(zw),1*ones(1,nr),"001");
	plot2d(real(zw),-imag(zw),1*ones(1,nr),"001");
	
	fig.immediate_drawing = immediate_drawing;
	
endfunction
