getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_mdialog.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('iir_gateway.sce');

exec(path+'iir_dial.sce');
[resp]=demo_mdialog(path+'iir_dial.bin');

demo_message(path+'iir_1.sce');

demo_run(path+'iir_1.sce');

demo_end();
