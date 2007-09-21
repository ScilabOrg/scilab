// Copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;


demolist=['Surface with holes 1','clf();showinstr(hole3d);hole3d();';
	  'Surface with holes 2','clf();showinstr(hole3d1);hole3d1();';
	  'Sphere','clf();showinstr(sphere);sphere();';
	  'Shell','clf();showinstr(shell);shell();';
	  'Spiral','clf();showinstr(spiral);spiral();';
	  'Rings','clf();showinstr(rings);rings();';
	  'Torus','clf();showinstr(torus);torus();';
	  'Torus 1','clf();showinstr(torus1);torus1();';
	  'Moebius','clf();showinstr(moebius);moebius();';
	  'Tube','clf();showinstr(tube);tube(50);';
	  'Black Hole','clf();showinstr(bh);bh(50);';
	  'Riemann surface (n=2)','clf();showinstr(cplxroot);cplxroot(2,20,130,45);']

getf('SCI/demos/graphics/more/Macros.sci');
exec('SCI/demos/graphics/more/surfaces.sci');

demo_function_choice();

demo_end();

