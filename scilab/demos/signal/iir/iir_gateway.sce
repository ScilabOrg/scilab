getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_mdialog.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('iir_gateway.sce');

exec('SCI/demos/signal/iir/iir_dial.sce');
[resp]=demo_mdialog('SCI/demos/signal/iir/iir_dial.bin');

demo_message('SCI/demos/signal/iir/iir_1.sce');

demo_run('SCI/demos/signal/iir/iir_1.sce');

demo_end();
