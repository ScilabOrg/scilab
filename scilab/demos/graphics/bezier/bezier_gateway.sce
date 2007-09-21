//copyright INRIA

exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_function_choice.sci');
exec('SCI/demos/demos_lib/demo_message.sci');

exec('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

exec('SCI/demos/graphics/bezier/Macros.sci');
exec('SCI/demos/graphics/bezier/bezier.sci','n');

demolist=[
	'Gammatest','clf();gammatest();';
	'2D curve Bezier test','clf();beziertest()';
	'3D curve Bezier test','clf();bezier3dtest()';
	'Bezier surface test','clf();beziersurftest()'
	'Bezier surface test 2','clf();c1test()']

demo_function_choice;

demo_end();
