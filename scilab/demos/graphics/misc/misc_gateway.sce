
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('misc_gateway.sce');

demo_message('SCI/demos/graphics/misc/misc_1.sce');

demo_run('SCI/demos/graphics/misc/misc_1.sce');

demo_end();
