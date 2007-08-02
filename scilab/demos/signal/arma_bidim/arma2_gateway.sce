getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('arma2_gateway.sce');

demo_run('SCI/demos/signal/arma_bidim/arma2_1.sce');

demo_end();
