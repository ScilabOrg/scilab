function [resp]=demo_mdialog(fil)
load(fil,'titl','namevar','value');

resp=x_mdialog(titl,namevar,value);
endfunction

