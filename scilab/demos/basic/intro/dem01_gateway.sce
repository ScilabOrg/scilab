getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('dem01_gateway.sce');

demo_run('SCI/demos/basic/intro/dem01_1.sce');

demo_end();
