getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('xdemo_gateway.sce');

demo_message('SCI/demos/graphics/bar/bar_1.sce');

demo_run('SCI/demos/graphics/bar/bar_1.sce');

demo_end();
