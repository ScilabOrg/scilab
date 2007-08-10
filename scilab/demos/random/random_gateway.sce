// Copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');

[path,oldln]=demo_begin('random_gateway.sce');

getf(path+'random.sci');
exec(path+'randomT.sci');

demolist=[
	'Simulation of a binomial random variable','set figure_style new;xbasc();BinomialT();';
	'Simulation of a discrete random variable','set figure_style new;xbasc();RndDiscT();';
	'Simulation of a geometric random variable','set figure_style new;xbasc();GeomT(1000);';
	'Simulation of a Poisson random variable','set figure_style new;xbasc();PoissonT() ;';
	'Simulation of an exponential random variable','set figure_style new;xbasc();ExpT();';
	'Simulation of a Weibull random variable','set figure_style new;xbasc();WeibullT();';
	'Simulation of an hyper geometric random variable','set figure_style new;xbasc();HyperGeomT();';
	'Simulation of an Erlang random variable','set figure_style new;xbasc();ErlangT();'];

demo_function_choice();

demo_end();
