exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

demo_message('SCI/demos/simulation/ode/ode_chimpl/ode_chimpl_2.sce');

demo_run('SCI/demos/simulation/ode/ode_chimpl/ode_chimpl_2.sce');

demo_end();

