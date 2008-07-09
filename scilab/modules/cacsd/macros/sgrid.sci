// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



function [] = sgrid(zeta,wn,col)

// sgrid()
// sgrid(Z,Wn)
// sgrid('new') 
[lhs,rhs]=argn(0)
if rhs < 3 ; col=3;end
if rhs==0 then 
	[a,limits]=xgetech()
	wmax = 10 .^(floor(log10(max(abs(limits)))));
	wn= 0:1:10;
	zeta = linspace(0,1,10);
end
if rhs==1 then 
	if type(zeta)<>10 then 
		write(%io(2),'sgrid with one argument : sgrid(''new'')')
		error(999)
		return
	end
	wn = 0:1:10;
        zeta = [ 0 .1 .2 .3 .4 .5 .6 .7 .8 .9 1 ];
	wmax = 10;
	xbasc();
	plot2d(0,0,0,"011"," ",[-20,-20,20,20]);
end
if rhs>= 2 then 
	[a,limits]=xgetech()
	wmax = 10 .^(floor(log10(max(abs(limits)))));
end 
	// building a grid 
	zx = 0:.01:1;
	[rx,cx]=size(wn);[ry,cy]=size(zx);
	w=wn.*.ones(cy,1);z=zx'.*.ones(1,cx);

	// plot : part I 
	re = - w .* z;
	[zr,zc] = size(z);
	im = w .* sqrt( 1 - z .* z );
	plot2d(re,im,col*ones(1,zc),"000");
	plot2d(re,-im,col*ones(1,zc),"000");
	
	// info on curves 

	[rer,rec] = size(re)

        axes = gca();
        axes.clip_state = "clipgrf";
	xnumb(re(1,:),im(1,:),wn);
        axes.clip_state = "off";

	// building an other grid 

	wn = [0,wn,2*wmax];
	[rx,cx]=size(wn);[ry,cy]=size(zeta);
	w=wn.*.ones(cy,1);z=zeta'.*.ones(1,cx);

	// plot part II  

	[zr,zc] = size(z);
	re = -w .* z;
	im = w .* sqrt( 1 - z .* z );
	plot2d(re',im',col*ones(1,zr),"000");
	plot2d(re',-im',col*ones(1,zr),"000");

	// info on each curve ( straight lines )
	[rer,rec] = size(re)
        axes.clip_state = "clipgrf";
	xnumb(re(:,$)',im(:,$)',zeta);
        axes.clip_state = "off";
	
endfunction
