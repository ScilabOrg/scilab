
// Copyright INRIA

exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_message.sci');


demo_begin;	

demo_message('SCI/demos/simulation/npend/n_pendulum.sci');

getf('SCI/demos/simulation/npend/n_pendulum.sci');
demo_pendulum()

demo_end();
