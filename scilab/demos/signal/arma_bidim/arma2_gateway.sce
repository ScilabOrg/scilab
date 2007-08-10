getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('arma2_gateway.sce');

demo_message(path+'arma2_1.sce');

demo_run(path+'arma2_1.sce');

demo_end();
