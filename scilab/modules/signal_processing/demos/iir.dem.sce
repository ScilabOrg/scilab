// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

ystr=[ 'type (lp, hp, bp, sb) ';
    'order (integer)';
    'design (butt, cheb1, cheb2 , ellip)' ;
    'cut-off frequencies (2-vector in [0, 0.5])';
    'error values  0 delta(1),delta(2) 1 '];
w=x_mdialog('Choose filter type and parameters',...
    ystr,['bp';'3';'ellip';'0.15 0.25';'0.08 0.035'])
if w<>[] then
  mode(1)	
  hz=iir(evstr(w(2)),w(1),w(3),evstr(w(4)),evstr(w(5)));
  [hzm,fr]=frmag(hz,256);

  my_handle = scf(100001);
  clf(my_handle,"reset");

  plot2d(fr',hzm');
  xtitle('Discrete IIR filter '+'( '+ w(3)+' ) ',' ',' ');
end
