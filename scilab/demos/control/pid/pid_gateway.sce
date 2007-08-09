getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_mdialog.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');

[path,oldln]=demo_begin('pid_gateway.sce');

demo_message(path+'pid_3.sce');

demo_run('SCI/demos/control/pid/pid_1.sce');

demo_message('SCI/demos/control/pid/pid_2.sce');	

demo_run('SCI/demos/control/pid/pid_3.sce');

demo_end();
