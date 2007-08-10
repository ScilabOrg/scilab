getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('ode_simple_gateway.sce');

demo_message(path+'ode_simple_1.sce');

demo_run(path+'ode_simple_1.sce');

demo_end();
