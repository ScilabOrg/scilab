exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

exec('SCI/demos/control/pendulum/simulation.sci')
exec('SCI/demos/control/pendulum/graphics.sci')
//
demo_run('SCI/demos/control/pendulum/pendule_1.sce');

demo_end();
