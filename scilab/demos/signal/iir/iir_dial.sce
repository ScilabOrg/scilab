titl='Choose filter type and parameters'
namevar=[ 'type (lp, hp, bp, sb) ';
    'order (integer)';
    'design (butt, cheb1, cheb2 , ellip)' ;
    'cut-off frequencies (2-vector in [0, 0.5])';
    'error values  0 delta(1),delta(2) 1 ']
value=['bp';'3';'ellip';'0.15 0.25';'0.08 0.035']

save('SCI/demos/signal/iir/iir_dial.bin',titl,namevar,value);
