// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for best_match function
// =============================================================================


ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
n=28;
g=make_graph('foo',0,n,ta,he);
xx=[46 120 207 286 366 453 543 544 473 387 300 206 136 250 346 408];
g('node_x')=[xx 527 443 306 326 196 139 264  55  58  46 118 513];
yy=[36  34  37  40  38  40  35 102 102  98  93  96 167 172 101 179];
g('node_y')=[yy 198 252 183 148 172 256 259 258 167 109 104 253];
show_graph(g);
[card,match] = best_match(g);
sp=sparse([ta' he'],[1:size(ta,2)]',[n,n]);
sp1=sparse([[1:n]' match'],ones(1,size(match,2))',[n,n]);
[ij,v,mn]=spget(sp.*sp1);
show_arcs(v');
//
// WITH A LARGER GRAPH
g=load_graph(SCI+"/modules/metanet/tests/unit_tests/mesh1000.graph");
g('directed')=0;
ta=g('tail');he=g('head');n=node_number(g);
show_graph(g,'new',1/2,[1000,400]);
[card,match] = best_match(g);
sp=sparse([ta' he'],[1:size(ta,2)]',[n,n]);
sp1=sparse([[1:n]' match'],ones(1,size(match,2))',[n,n]);
[ij,v,mn]=spget(sp.*sp1);
show_arcs(v');

