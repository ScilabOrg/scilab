// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

x = linspace(0,2*%pi,11);
y = sin(x);
xx = linspace(-2*%pi,4*%pi,400)';
yy = linear_interpn(xx, x, y, "periodic");
assert_checkequal(size(yy), [400 1]);

n = 8;
x = linspace(0,2*%pi,n); y = x;
z = 2*sin(x')*sin(y);
xx = linspace(0,2*%pi, 40);
[xp,yp] = ndgrid(xx,xx);
zp = linear_interpn(xp,yp, x, y, z);
assert_checkequal(size(zp), [40 40]);

nx = 20; ny = 30;
x = linspace(0,1,nx);
y = linspace(0,2, ny);
[X,Y] = ndgrid(x,y);
z = 0.4*cos(2*%pi*X).*cos(%pi*Y);
nxp = 60 ; nyp = 120;
xp = linspace(-0.5,1.5, nxp);
yp = linspace(-0.5,2.5, nyp);
[XP,YP] = ndgrid(xp,yp);
zp1 = linear_interpn(XP, YP, x, y, z, "natural");
assert_checkequal(size(zp1), [60 120]);

zp2 = linear_interpn(XP, YP, x, y, z, "periodic");
assert_checkequal(size(zp1), [60 120]);

zp3 = linear_interpn(XP, YP, x, y, z, "C0");
assert_checkequal(size(zp1), [60 120]);

zp4 = linear_interpn(XP, YP, x, y, z, "by_zero");
assert_checkequal(size(zp1), [60 120]);

zp5 = linear_interpn(XP, YP, x, y, z, "by_nan");
assert_checkequal(size(zp1), [60 120]);



