getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');


[path,oldln]=demo_begin('remezfilt_gateway.sce');

demo_message('SCI/demos/signal/minimax/remezfilt_1.sce');

demo_run('SCI/demos/signal/minimax/remezfilt_1.sce');

demo_end();
