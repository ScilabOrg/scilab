
// Copyright ENPC INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_message.sci');

demo_begin;	

demo_message('SCI/demos/simulation/npend/n_pendulum.sci');

getf('SCI/demos/simulation/npend/n_pendulum.sci');
demo_pendulum()

demo_end();
