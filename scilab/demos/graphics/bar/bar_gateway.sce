getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('bar_gateway.sce');

demo_message(path+'bar_1.sce');

demo_run(path+'bar_1.sce');

demo_end();
