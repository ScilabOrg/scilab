//copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

getf('SCI/demos/graphics/bezier/Macros.sci');
exec('SCI/demos/graphics/bezier/bezier.sci','n');

demolist=[
	'Gammatest','xbasc();gammatest();showinstr(gammatest);';
	'2D curve Bezier test','xbasc();beziertest();showinstr(beziertest);';
	'3D curve Bezier test','xbasc();bezier3dtest();showinstr(bezier3dtest);';
	'Bezier surface test','xbasc();beziersurftest();showinstr(beziersurftest);'
	'Bezier surface test 2','xbasc();c1test();showinstr(c1test);']

demo_function_choice;

demo_end();
