// Copyright ENPC

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_function_choice.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('xdemo_gateway.sce');

exec(path+'xdemo_1.sce');

demo_run(path+'xdemo_2.sce');

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
