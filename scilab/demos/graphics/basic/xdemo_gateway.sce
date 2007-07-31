// Copyright ENPC

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('xdemo.dem');

exec('SCI/demos/graphics/basic/xdemo-1.dem');

demolist=[
    'Rectangles, Arcs, Polylines, Segments and Numbers','xdemo1()';
    'Rectangles, Arcs, Polylines and Strings',          'xdemo2()';
    'Setting properties','xdemo3()';
    'Window properties','xdemo4()';
    'Set of rectangles','xdemo5()';
    'Colored matrix','xdemo6()';
    'Colored histogram','xdemo7()';
    'Pie Chart','xdemo8()' ]

demo_function_choice;

demo_end();
