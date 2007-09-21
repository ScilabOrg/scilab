// Copyright INRIA
mode(0);

num=x_message([' ';
'METANET is a Scilab toolbox for graph and network analysis';
'It comes with a graphics tool for visualizing and creating graphs';
' ';
'This is a demo for finding the best and fastest way';
'  to go from one station to another in Paris subway';
' ';
'Try the demo?'],['Yes','No']);

if num==1 then
  demo_message('SCI/demos/metanet/metro.sci');
  metro()
end
