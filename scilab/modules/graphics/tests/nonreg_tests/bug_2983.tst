// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2983 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2983
//
// <-- Short Description -->
// If figure_size property is modified more than one time, next changes have no effect on the figure.
function [z]=__milk_drop(x,y)
    sq=x.^2+y.^2;
    z= exp( exp(-sq).*(exp(cos(sq).^20)+8*sin(sq).^20+2*sin(2*(sq)).^8) );
endfunction

x=-2:0.1:2; y=x;
z=eval3d(__milk_drop,x,y);
plot3d(x,y,z)


a=gca();
a.children(1).hiddencolor
a.children(1).hiddencolor=5;

// check that no hidden facets appears.
// Might be updated when image processing will be integrated in Scilab.
// Export and check that no hidden color appears on the image
