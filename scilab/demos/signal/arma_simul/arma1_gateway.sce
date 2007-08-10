getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('arma1_gateway.sce');

demo_message(path+'arma1_1.sce');

demo_run(path+'arma1_1.sce');

demo_end();

