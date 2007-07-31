
// Copyright ENPC INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_message.sci');

[path,oldln]=demo_begin('npend_gateway.sce')	

demo_message(path+'n_pendulum.sci');

getf(path+'n_pendulum.sci');
demo_pendulum()

demo_end();
