getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

exec('SCI/demos/graphics/plots/dessin.sci');

demolist=['plot2d','str_1()';
	'plot2d1','str_2()';     
	'plot2d3','str_3()';
	'plot2d1','str_4()';     
	'histplot','str_5()';
	'fplot2d','str_6()';
	'param3d','str_7()';
	'param3d','str_8()';
	'plot3d','str_9()';
	'fplot3d','str_10()';
	'plot3d1','str_11()';
	'fplot3d1','str_12()';
	'contour','str_13()';
	'fcontour','str_14()'; 
	'contourf','str_15()';
	'champ','str_16()';
	'fchamp','str_17()';
	'grayplot','str_18()';
	'fgrayplot','str_19()';
	'errbar','str_20()';
	'abaque','str_21()';
	'zgrid','str_22()';
	'xgeom','str_23()';
	'contour (3d) 2','str_24()';
	'contour (3d) 3','str_25()';
	'contour (3d) 4','str_26()';
	'subplot','str_27()';
	'fac3d','str_28()';
	'fac3d1','str_29()'];

demo_function_choice;

demo_end();
