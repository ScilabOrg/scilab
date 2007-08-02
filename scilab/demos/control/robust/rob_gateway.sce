getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('rob_gateway.sce');

demo_run('SCI/demos/control/robust/rob_1.sce');

demo_end();
