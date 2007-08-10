getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('ode_lotka_gateway.sce');

demo_message(path+'ode_lotka_1.sce');

demo_message(path+'ode_lotka_2.sce');

demo_run(path+'ode_lotka_2.sce');

demo_end();
