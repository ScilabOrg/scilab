getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_mdialog.sci');

[path,oldln]=demo_begin('mixed_gateway.sce');

demo_message(path+'mixed_3.sce');

demo_run('SCI/demos/control/mixed/mixed_1.sce');

demo_message('SCI/demos/control/mixed/mixed_2.sce');

demo_run('SCI/demos/control/mixed/mixed_3.sce');

demo_end();
