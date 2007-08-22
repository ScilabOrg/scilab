getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

getf('SCI/demos/control/pendulum/simulation.sci')
getf('SCI/demos/control/pendulum/graphics.sci')
//
demo_run('SCI/demos/control/pendulum/pendule_1.sce');

demo_end();
