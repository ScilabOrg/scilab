getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('SCI/demos/simulation/ode/ode_1dvect/ode_1dvect_gateway.sce');

demo_message('SCI/demos/simulation/ode/ode_1dvect/ode_1dvect_1.sce');

demo_message('SCI/demos/simulation/ode/ode_1dvect/ode_1dvect_2.sce');

demo_run('SCI/demos/simulation/ode/ode_1dvect/ode_1dvect_2.sce');

demo_end();
